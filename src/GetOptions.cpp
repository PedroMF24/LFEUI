#include "GetOptions.h"

Options::Options() {
	h_opt = d_opt = t_opt = m_opt = M_opt = p_opt = 0;
	minVal = maxVal = 0;
	title.clear();
	path.clear();
	dir.clear();

	help_message = "\nHELP MESSAGE\n\n"
					"How to use different arguments:\n"
					"-t <TITLE>:\tDefines the title, no option gives blank title.\n\t\tTo write multiple words as title, use -t <'First and Second Words'>";
}


int Options::isString(string str) {
	for (int i = 0; i < str.length(); i++)
	if (isdigit(str[i]) == 0)
		return 0;
		return 1;
}

void Options::SetDir(string newDir) {
	dir = newDir;
}

void Options::SetPath(string newPath) {
	path = newPath;
}

void Options::SetTitle(string newTitle) {
	title = newTitle;
}

const char* Options::GetTile() {
	return title.c_str();
}

int Options::getOptions(int argc, char **argv) 
{
	int i, c;
	const char *options = "hd:t:m:M:p:";
	string tempDir, tempPath, tempTitle;
	// Structure which would store the metadata
	struct stat sb;

	while ((c = getopt(argc, argv, options)) != -1 ) {
		switch (c)
		{
			// help
			case 'h':
				h_opt = 1;
				break;
			// destiny folder
			case 'd':
				tempDir = optarg;
				SetDir(tempDir);
				cout << "Output saved in " << tempDir << endl;
				d_opt = 1;
				// if (sscanf(optarg, "%s", ) != 1) {
				// 	fprintf(stderr, "**'%s' is not a valid output directory\n", optarg);
                //     return -1;
				// }
				// if (stat(tempDir.c_str(), &sb) == 0) {
				// 	cout << "Temp Output saved in " << tempDir << endl;
				// 	cout << "Dir before Output saved in " << dir << endl;
				// 	SetDir(tempDir);
				// 	cout << "Dir after set Output saved in " << dir << endl;
				// 	d_opt = 1;
				// }				
				// else
				// 	cout << "The Path is invalid!";
				break;
			case 't':
				// if (sscanf(optarg, "%s", &title[0]) != 1) {
				// 	fprintf(stderr, "**'%s' is not a valid title\n", optarg);
				// 	return -1;
				// }
				tempTitle = optarg;
				SetTitle(tempTitle);
				cout << "Output title is " << title << endl;
				t_opt = 1;
				// if (isString(title)) {
				// 	t_opt = 1;
				// }
				break;
			case 'm':
				if (sscanf(optarg, "%lf", &minVal) != 1) {
					fprintf(stderr, "**'%s' is not a valid minimum\n", optarg);
					return -1;
				}
				m_opt = 1;
				break;
			case 'M':
				if (sscanf(optarg, "%lf", &maxVal) != 1) {
					fprintf(stderr, "**'%s' is not a valid maximum\n", optarg);
					return -1;
				}
				M_opt = 1;
				break;
			case 'p':
				{
					// if (sscanf(optarg, "%s", &path[0]) != 1) {
					// 	fprintf(stderr, "**'%s' is not a valid path\n", optarg);
					// 	return -1;
					// }
					// cout << "O path é " << path << "Optarg " << optarg << endl;
					tempPath = optarg;
					SetPath(tempPath);
					cout << "Path is " << path << endl;
					ifstream file(path);
					// Check if the file was successfully opened
					if (!file.is_open()) {
						fprintf(stderr, "**'%s' is not a valid path to file\n", optarg);
						return -1;
					}
					file.close();
					p_opt = 1;
					break;
				}
			case '?':
                if (strchr("dtmMp", optopt) != NULL) {
                    fprintf(stderr, "**Option '-%c' needs an argument\n", optopt);
                    return -1;
                }
                fprintf(stderr, "**Option '-%c' is invalid\n", optopt);
                return -1;
            //
            default:
                // fprintf(stderr, "We must never get here!\n");
                assert(!"We must never get here!");
				break;
		}
	}
	return 0;
}