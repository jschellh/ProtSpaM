#include "parameters.h"

using namespace std;

void printHelp(){
    string help =
    "\nUsage: ./protfswm [options] <sequence> "
    "\n"
    "\n<sequence> format:"
    "\n\t Sequence must be in FASTA format. All concatenated proteomes must be contained in one FASTA file. Example:"
    "\n\t >Proteome1"
    "\n\t ARNDCQE.."
    "\n\t >Proteome2"
    "\n\t ARNDCQE.."
    "\n\t >Proteome3"
    "\n\t ARNDCQE.."
    "\n\t .."
    "\n\t "
    "\nOptions:"
    "\n\t -h/-?: print this help and exit"
    "\n\t -k <integer>: pattern weight (default 8)"
    "\n\t -d <integer>: number of don't-care positions (default 50)"
    "\n\t -s <integer>: the minimum score of a spaced-word match to be considered homologous (default: 0)"
    "\n\t -p <integer>: number of patterns used (default 1)"
    "\n\t -t <integer>: number of threads (default: omp_get_max_threads() )"
    "\n";
	cout << help << endl;
}

void parseParameters(int argc, char *argv[], int& weight, int& dc, int& threshold, int& patterns, int& threads){
	int option_char;
	 while ((option_char = getopt (argc, argv, "k:d:s:p:t:h")) != -1){
		switch (option_char){
			case 'k':
				weight = atoi (optarg);
				if(weight<4 || weight > 12){
					std::cerr << "Weight '-k' must be between 4 and 12"<< std::endl;
					exit (EXIT_FAILURE);
				}
				break;
            case 'd':
                dc = atoi (optarg);
                if (dc < 1){
					std::cerr << "number of don't-care positions '-d' must be an integer larger than 0"<< std::endl;
					exit (EXIT_FAILURE);
                }
                break;
			case 's':
				threshold = atoi (optarg);
				break;
           	case 'p':
				patterns = atoi (optarg);
				if (patterns < 1 || patterns > 10){
					std::cerr << "Number of patterns '-p' must be an integer between 1 and 10"<< std::endl;
				}
				break;
			case 't':
				threads = atoi (optarg);
				if(threads<1){
					std::cerr << "threads '-t' must be an integer larger than 0"<< std::endl;
					exit (EXIT_FAILURE);
				}
				break;
			case 'h':
				printHelp();
				exit (EXIT_SUCCESS);
				break;
			case '?':
				printHelp();
				exit (EXIT_FAILURE);
      	}
	}
}

void printParameters(int& weight, int& dc, int& threshold, int& patterns, int& threads) {
    cout << "-----Parameters used------\n";
    cout << "Weight: " << weight << endl;
    cout << "Number of don't-care positions: " << dc << endl;
    cout << "Threshold: " << threshold << endl;
    cout << "Number of patterns: " << patterns << endl;
    cout << "Number of threads: " << threads << endl;
    cout << "--------------------------\n";
}
