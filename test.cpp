#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

std::string filename = "typing_test_results.csv";

// I haven't fully implemented this feature yet
enum test_type { QUOTE, ENG25K };

class typing_test {
   private:
    int failed_attempts = 0;
    float wpm, accuracy;
    time_t date;
    enum test_type ttype;
    bool abandoned = false;

   public:
    typing_test() { time(&date); }

    void add_attempt() { failed_attempts++; }
    void set_accuracy(float input_acc) { accuracy = input_acc; }
    void set_wpm(float input_wpm) { wpm = input_wpm; }
    void set_type(enum test_type input_ttype = QUOTE) { ttype = input_ttype; }

    void print_test() const {
        char output[9];
        std::tm* fdate = std::localtime(&date);
        std::strftime(output, 9, "%D", fdate);
        std::cout << "wpm: " << std::fixed << std::setprecision(1) << wpm
                  << ", accuracy: " << std::setprecision(2) << accuracy
                  << "\nfailed attempts: " << failed_attempts
                  << " abandoned ? -> " << abandoned << "\ndate : " << output
                  << std::endl;
    }

    // to store the tests in a text file, we will need a condensed format, which
    // will be the string: wpm accuracy failed_attempts abandoned day month year
    // all separated by ";"
    std::string test_to_string() const {
        char str_date[9];
        std::tm* fdate = std::localtime(&date);
        std::strftime(str_date, 9, "%d,%m,%y", fdate);
        std::string output;
        output += std::to_string((wpm)).substr(0, 4) + "," +
                  std::to_string((accuracy)).substr(0, 5) + "," +
                  std::to_string((failed_attempts)) + "," +
                  std::to_string(static_cast<int>((abandoned))) + "," +
                  str_date + "\n";
        return output;
    }

    // adds test to the csv
    void save_test(const std::string& csv_file_name) const {
        std::ofstream file;
        file.open(csv_file_name, std::ios::out | std::ios::app);
        file << test_to_string();
        file.close();
    }

    void start_test() {
        int input = 0;
        std::string help_text =
            "1: add attempt, 2: test failure, 3: test successful\n";
        std::cout << help_text;
        while (input <= 1) {
            std::cin >> input;
            switch (input) {
                case 1:
                    add_attempt();
                    std::cout << "attempt : " << failed_attempts << std::endl;
                    break;
                case 2:
                    abandoned = true;
                    wpm       = 0;
                    accuracy  = 0;
                    break;
                case 3:
                    std::cout << "insert wpm: ";
                    std::cin >> wpm;
                    std::cout << "insert accuracy: ";
                    std::cin >> accuracy;
                    if (accuracy > 100.01) {
                        std::cout << "give accuracy is not possible, please "
                                     "enter again :";
                        std::cin >> accuracy;
                    }
                    break;
                default:
                    std::cout << "given value does not fit schema, please "
                                 "refer to :\n"
                              << help_text;
                    input = 0;
                    break;
            }
        }
    }

    void test_series(const std::string& csv_file_name) {
        std::string answer = "y";
        while (answer == "y") {
            start_test();
            save_test(csv_file_name);
            failed_attempts = 0;
            abandoned       = false;
            std::cout << "do you wish to perform another test? (y/n)    \n";
            std::cin >> answer;
        }
    }
};

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void make_typing_test_csv(const std::string& csv_file_name) {
    std::ofstream file(csv_file_name);
    if (!file) {
        std::cerr << "could not create csv file\n";
    }
    file << "wpm,accuracy,failed attempts,abandoned,day,month,year modified\n";
    file.close();
}

int main() {
    typing_test tt;
    if (!fileExists(filename)) {
        make_typing_test_csv(filename); 
    }
    tt.test_series(filename);
    return 0;
}