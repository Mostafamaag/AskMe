
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <assert.h>

using namespace std;

//helper functions
int ReadInt(int low, int high) {
	int num;
	cout << "Please enter number between " << low << "-" << high << ":";
	cin >> num;
	if (num >= low && num <= high) {
		return num;
	}
	cout << "Invalid number!\n";
	return ReadInt(low, high);

}

vector<string> LoadData(const string &fileName) {
	ifstream myfile;
	vector<string> record;
	myfile.open(fileName);
	string s;
	while (myfile) {
		getline(myfile, s);
		record.push_back(s);
	} // get extra empty line;
	record.pop_back();
	myfile.close();

	return record;
}

void SaveData(vector<string> record, const string &fileName,
		bool append = true) {

	ofstream myfile;
	if (append)
		myfile.open(fileName, ios_base::app);
	else
		myfile.open(fileName);

	if (myfile.fail()) {
		cout << "\nError! file not exist";
		return;
	}
	for (auto rec : record) {
		myfile << rec << "\n"; // extra empty line at the end of file need to pop
	}
	myfile.close();
}

vector<string> Split(const string &str) {
	stringstream ss(str);
	vector<string> v;
	string s;
	while (getline(ss, s, ',')) {
		v.push_back(s);
	}
	return v;
}
void ShowMenu(vector<string> menu) {
	for (auto m : menu) {
		cout << m << endl;
	}
}
//---------------------------------------------------------------------
class Question {
private:
	int question_id;
	int parent_question_id;
	int from_user_id;    // changed to from username instead of id
	int to_user_id;		// changed to to username instead of id
	string from_username;
	string to_username;
	bool is_anonymous_questions;
	string question_text;
	string answer_text;
public:
	Question() {
		question_id = parent_question_id = from_user_id = to_user_id = -1;
		is_anonymous_questions = 1;
		from_username = to_username = question_text = answer_text = "";
	}
	Question(const string &question) {
		vector<string> data = Split(question);

		//assert(data.size() == 6 );
		question_id = stoi(data[0]);
		parent_question_id = stoi(data[1]);
		from_username = data[2];
		to_username = data[3];
		is_anonymous_questions = stoi(data[4]);
		question_text = data[5];

		if (data.size() == 6)
			answer_text = "";
		else
			answer_text = data[6];
		from_user_id = -1;
		to_user_id = -1;

	}

	string ToString() const {
		ostringstream oss;
		oss << question_id << ',' << parent_question_id << ',' << from_username
				<< ',' << to_username << ',' << is_anonymous_questions << ','
				<< question_text << ',' << answer_text;
		return oss.str();
	}
	void PrintQuestionInformation() const {
		if (parent_question_id != -1)
			cout << "Thread Parent Question ID (" << parent_question_id
					<< ") - ";

		cout << "Question Id(" << question_id << ")";
		if (!is_anonymous_questions)
			cout << " from username(" << from_username << ")";
		else
			cout << " from (anonymous)";
		cout << " To username(" << to_username << ")";

		cout << "\n\tQuestion: " << question_text << "\n";
		if (answer_text != "")
			cout << "\tAnswer  : " << answer_text << "\n";
		cout << endl;
	}
	void PrintToMeQuestion() const {

		if (parent_question_id != -1) {
			cout << "\t-Thread- ";

		}
		cout << "Question id(" << getQuestionId() << ")";
		if (!isAnonymousQuestions()) {
			cout << " From(" << getFromUsername() << ")";
		} else
			cout << " From(Anonymous)";
		cout << "\n\tQuestion : " << getQuestionText();
		cout << "\n\tAnswer   : " << getAnswerText() << "\n";
		cout << "\n";
	}
	void PrintFromMeQuestions() const {
		cout << "Question id(" << getQuestionId() << ")";
		if (isAnonymousQuestions())
			cout << " AQ";
		cout << " to(" << getToUsername() << ")";
		cout << "\n\tQuestion : " << getQuestionText();
		cout << "\n\tAnswer   : " << getAnswerText();
		cout << endl;
	}

	int getFromUserId() const {
		return from_user_id;
	}

	void setFromUserId(int fromUserId) {
		from_user_id = fromUserId;
	}

	bool isAnonymousQuestions() const {
		return is_anonymous_questions;
	}

	void setIsAnonymousQuestions(bool isAnonymousQuestions) {
		is_anonymous_questions = isAnonymousQuestions;
	}

	int getToUserId() const {
		return to_user_id;
	}

	void setToUserId(int toUserId) {
		to_user_id = toUserId;
	}

	int getParentQuestionId() const {
		return parent_question_id;
	}

	void setParentQuestionId(int parentQuestionId) {
		parent_question_id = parentQuestionId;
	}

	const string& getFromUsername() const {
		return from_username;
	}

	void setFromUsername(const string &fromUsername) {
		from_username = fromUsername;
	}

	const string& getToUsername() const {
		return to_username;
	}

	void setToUsername(const string &toUsername) {
		to_username = toUsername;
	}

	const string& getAnswerText() const {
		return answer_text;
	}

	void setAnswerText(const string &answerText) {
		answer_text = answerText;
	}

	int getQuestionId() const {
		return question_id;
	}

	void setQuestionId(int questionId) {
		question_id = questionId;
	}

	const string& getQuestionText() const {
		return question_text;
	}

	void setQuestionText(const string &questionText) {
		question_text = questionText;
	}
};



class User {
private:
	int id;
	string name;
	string password;
	string username;
	string email;
	int allow_anonymous_questions;

	vector<int> questions_id_from_me;
	map<int, vector<int>> threads_map; // parent question id to questions ids map


public:

	User() {
			id = -1;
			allow_anonymous_questions = 1 ;

	}
	User(const string &record) {
		vector<string> data = Split(record);

		id = stoi(data[0]);
		name = data[1];
		password = data[2];
		username = data[3];
		email = data[4];
		allow_anonymous_questions = stoi(data[5]);
	}

	void DeleteQuestion(int id) {
		for (auto i : threads_map) {
			if (id == i.first)
				threads_map.erase(id);
		}
	}

	void SignUp(const string &user_name,int last_id) {

		setUsername(user_name);
		setId(last_id);
		string str;

		cout << "Enter your name: ";
		cin >> str;
		setName(str);

		cout << "Enter your password: "; // next re enter & weak- strong
		cin >> str;
		setPassword(str);

		cout << "Enter your Email: "; // next handle valid mail
		cin >> str;
		setEmail(str);

		int aq;
		cout << "Allow anonymous questions? 0-1: ";
		cin >> aq;
		setAllowAnonymousQuestions(aq);

	}

	void RestFromMeQuestions(const vector<int> &questions_from_me) {
		questions_id_from_me.clear();
		for (const auto &question_id : questions_from_me) {
			questions_id_from_me.push_back(question_id);
		}
	}

	void RestToMeQuestions(const vector<pair<int, int>> &questions_to_me) {
		threads_map.clear();
		for (const auto &p : questions_to_me) {
			threads_map[p.first].push_back(p.second);
		}
	}

	const string& getName() const {
		return name;
	}
	void setName(const string &name) {
		this->name = name;
	}
	const string& getUsername() const {
		return username;
	}
	void setUsername(const string &username) {
		this->username = username;
	}
	const string& getEmail() const {
		return email;
	}
	void setEmail(const string &email) {
		this->email = email;
	}
	const string& getPassword() const {
		return password;
	}
	void setPassword(const string &password) {

		this->password = password;
	}

	void PrintUserInformation() const {
		cout << "Name :" << getName() << endl;
		cout << "Username :" << getUsername() << endl;
		cout << "Email :" << getEmail() << endl;
	}
	string ToString() const {
		ostringstream oss;
		oss << id << ',' << name << ',' << password << ',' << username << ','
				<< email << ',' << allow_anonymous_questions;
		return oss.str();
	}

	int getAllowAnonymousQuestions() const {
		return allow_anonymous_questions;
	}

	void setAllowAnonymousQuestions(int allowAnonymousQuestions) {
		allow_anonymous_questions = allowAnonymousQuestions;
	}

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	const vector<int>& getQuestionsIdFromMe() const {
		return questions_id_from_me;
	}

	void setQuestionsIdFromMe(const vector<int> &questionsIdFromMe) {
		questions_id_from_me = questionsIdFromMe;
	}

	const map<int, vector<int> >& getThreadsMap() const {
		return threads_map;
	}

	void setThreadsMap(const map<int, vector<int> > &threadsMap) {
		threads_map = threadsMap;
	}
};

class QuestionsManager {
private:

	int last_id;
	map<int, vector<int>> threads_map; 				//question id to questions ids
	map<int, Question> questionId_to_question_map; //question id to question object

public:
	QuestionsManager() {
		last_id=0;
	}
	void printThreads() {
		for (auto parent : threads_map) {
			cout << parent.first << " : ";
			for (auto child : parent.second) {
				cout << child << " ";
			}
			cout << endl;
		}
	}

	void PrintIdQuestions() {
		for (auto i : questionId_to_question_map) {
			cout << "#" << i.first << endl;
		}
	}

	void LoadDatabase() {
		questionId_to_question_map.clear();
		threads_map.clear();
		last_id=0;

		vector<string> records = LoadData("questions.txt");

		for (auto &record : records) {

			Question q(record);
			last_id = max(last_id,q.getQuestionId());
			questionId_to_question_map[q.getQuestionId()] = q;

			if (q.getParentQuestionId() == -1) {
				threads_map[q.getQuestionId()].push_back(q.getQuestionId());
				//cout << " -1" <<endl;
			} else {
				threads_map[q.getParentQuestionId()].push_back(
						q.getQuestionId());
				//cout<< "1" <<endl;
			}
		}

	}

	vector<int> GetFromMeQuestions(const User &user) {
		vector<int> question_ids;
		for (const auto &p : threads_map) {
			for (const auto question_id : p.second) {

				const Question question = questionId_to_question_map.find(
						question_id)->second;
				if (question.getFromUsername() == user.getUsername()) {
					//cout << "#Test :"<<question.getQuestionText() <<endl;

					question_ids.push_back(question.getQuestionId());
				}
			}
		}
		return question_ids;
	}

	vector<pair<int, int>> GetToMeQuestions(const User &user) {
		vector<pair<int, int>> question_ids;
		for (const auto &p : threads_map) {
			for (const auto &question_id : p.second) {

				const Question question = questionId_to_question_map.find(
						question_id)->second;
				if (question.getToUsername() == user.getUsername()) {
					//cout << "#Test :"<<question.getQuestionText() <<endl;
					if (question.getParentQuestionId() == -1) {
						question_ids.push_back(
								make_pair(question.getQuestionId(),
										question.getQuestionId()));
					} else {
						question_ids.push_back(
								make_pair(question.getParentQuestionId(),
										question.getQuestionId()));
					}
				}

			}
		}
		return question_ids;
	}

	void PrintFromMeQuestions(const User &user) const {

		cout << endl;
		if (user.getQuestionsIdFromMe().size() == 0) {
			cout << "No Questions!";
		}
		for (const auto &question_id : user.getQuestionsIdFromMe()) {

				const Question &question = questionId_to_question_map.find(question_id)->second;
					question.PrintFromMeQuestions();
					cout << endl;


			}
		cout << endl;
	}

	void PrintToMeQuestions(const User &user) {

		cout << endl;

		if (user.getThreadsMap().empty()) {
			cout << "No Questions!";
		}
		for (const auto &p : user.getThreadsMap()) {
			for (const auto &question_id : p.second) {

				const Question question = questionId_to_question_map.find(
						question_id)->second;
					//cout <<"parent : " << question.getParentQuestionId() <<endl;
					question.PrintToMeQuestion();
			}

		}

	}

	//used to answer my questions
	int ReadQuestionIdAny(const User &user) {
		int question_id;
		cout << "Enter question id or -1 to cancel :";
		cin >> question_id;
		if (question_id == -1)
			return -1;

		//check question is exist
		if (questionId_to_question_map.find(question_id)
				== questionId_to_question_map.end()) {
			cout << "\nERROR: No question with such ID. Try again\n\n";
			return ReadQuestionIdAny(user);
		}

		//check user have this question or not
		const Question &question =
				questionId_to_question_map.find(question_id)->second;

		if (question.getToUsername() != user.getUsername()) {
			cout << "\nERROR: Invalid id. Try again\n\n";
			return ReadQuestionIdAny(user);
		}
		return question_id;
	}

	int ReadQuestionIdThread(const User& user){
		int question_id;
		cout <<"For thread : Enter thread id (Head question id) or -1 to cancel :";
		cin >> question_id;
		if(question_id == -1) return -1;

		if(!threads_map.count(question_id)){
			cout << "No thread question with such ID! Please try again\n";
			return ReadQuestionIdThread(user);
		}
		return question_id;
	}

	void AnswerQuestion(const User &user) {
		int question_id = ReadQuestionIdAny(user);
		if (question_id == -1)
			return;

		cout << "Question :"
				<< questionId_to_question_map[question_id].getQuestionText();
		Question &question =
				questionId_to_question_map.find(question_id)->second;
		if (question.getAnswerText() != "") {
			cout << "\nWarning: Qestion already answered";
			cout << "\nOverwrite it? y-n :";
			char ch;
			cin >> ch;
			if (ch == 'n')
				return;
		}
		cout << "\nEnter your answer :";
		string ans;
		getline(cin, ans);
		getline(cin, ans);
		question.setAnswerText(ans);
		UpdateDatabase();
	}

	void DeleteQuestion(const User &user) {
		int question_id = ReadQuestionIdAny(user);
		if (question_id == -1)
			return;

		cout << "Question :"
				<< questionId_to_question_map[question_id].getQuestionText();

		//find if thread(parent) or not
		//get child questions
		vector<int> ids_to_remove;
		if(threads_map.count(question_id)){
			ids_to_remove = threads_map[question_id];
			threads_map.erase(question_id);
		}
		else{
			//remove from threads
			ids_to_remove.push_back(question_id);
			for(auto &pair : threads_map){
				vector<int> &vec = pair.second;
				for(int i = 0 ; i < (int)vec.size(); i++){
					if(question_id==vec[i]){
						vec.erase(vec.begin()+i);
						break;
					}
				}
			}
		}

		for(auto &id : ids_to_remove)
		 questionId_to_question_map.erase(id);

		cout << " 'Was deleted successfly' ";
	}

	void AskQuestion(const User &user, const pair<string, int> &to_user) {
		Question q;

		if (to_user.second == 0) {
			cout << "This user don't allow anonymous question\n";
			q.setIsAnonymousQuestions(0);
		} else {
			cout << "Is anonyous question? y-n :";
			char ch;
			cin >> ch;
			if (ch == 'y')
				q.setIsAnonymousQuestions(1);
			//else q.setIsAnonymousQuestions(0);
		}
		q.setParentQuestionId(ReadQuestionIdThread(user));
		string str;
		cout << "\nEnter your Question :";
		getline(cin, str);
		getline(cin, str);
		q.setQuestionText(str);
		q.setToUsername(to_user.first);
		q.setFromUsername(user.getUsername());
		q.setQuestionId(last_id++); // bug in parallel sessions
		questionId_to_question_map[q.getQuestionId()] = q;
		if (q.getParentQuestionId() == -1) {
			threads_map[q.getQuestionId()].push_back(q.getQuestionId());
		} else {
			threads_map[q.getParentQuestionId()].push_back(q.getQuestionId());
		}
	//	UpdateDatabase();

	}

	void UpdateDatabase() {
		vector<string> record;
		for (auto &i : questionId_to_question_map) {
			record.push_back(i.second.ToString());
			SaveData(record, "questions.txt", false);
		}

	}

	void ListFeed() const {
		for (auto &p : questionId_to_question_map) {
			const Question question_obj = p.second;
			if (question_obj.getAnswerText() != "")
				question_obj.PrintQuestionInformation();
		}
	}

};

class UsersManger {
private:
	User current_user;
	map<string, User> username_to_user_map; // username to user object
	int last_id;
public:
	UsersManger(){
		last_id =0;
	}
	void SignUp() {
		LoadDatabase();

		User user;
		string username;

		while (true) {
			cout << "Enter your username (No spaces) :";
			cin >> username;
			if (username_to_user_map.count(username)) {
				cout << "Already used. Try again\n";
				continue;
			}
			break;
		}
		current_user.SignUp(username,++last_id);
		username_to_user_map[current_user.getUsername()] = current_user;
		UpdateDatabas(current_user);
		Login();

	}
	void Login() {

		while (true) {
			LoadDatabase();
			string username;
			string pass;

			while (true) {
				cout << "\nEnter you username :";
				cin >> username;
				if (!username_to_user_map.count(username))
					cout << "\nInvaild username, Please try again!";
				else
					break;

			}
			const User &user = username_to_user_map.find(username)->second;

			while (true) {
				cout << "\nEnter your password :";
				cin >> pass;
				if (pass != user.getPassword())
					cout << "\nWrong password, Please try again!";
				else
					break;
			}
			current_user = user;
			break;

		}

	}
	void LoadDatabase() {
		last_id =0;
		username_to_user_map.clear();

		vector<string> records = LoadData("users.txt");
		for ( auto &record : records) {

			User user(record);
			last_id = max(last_id,user.getId());
			username_to_user_map[user.getUsername()] = user;

		}
	}
	void AccessSystem() {
		vector<string> systemMenu;
		systemMenu.push_back("-------- System Menu --------");
		systemMenu.push_back("1.Login");
		systemMenu.push_back("2.Sign Up");
		ShowMenu(systemMenu);
		int input = ReadInt(1, 2);
		if (input == 1) {
			Login();
		}
		if (input == 2) {
			SignUp();
		}

	}

	void ListSystemUsers() const {
		for ( auto &pair : username_to_user_map ){
			cout << "name: " << pair.second.getName()<< "\n";
			cout << "Username: " << pair.second.getUsername() << "\n\n";

		}
	}

	void UpdateDatabas(const User &user) {
		string data = user.ToString();
		vector<string> record;
		record.push_back(data);
		SaveData(record, "users.txt");
	}

	const User& getCurrentUser() const {
		return current_user;
	}

	void setCurrentUser(const User &currentUser) {
		current_user = currentUser;
	}
	void RestFromMeQuestions(const vector<int> questions) {
		current_user.RestFromMeQuestions(questions);
	}
	void RestToMeQuestions(const vector<pair<int, int>> questions) {
		current_user.RestToMeQuestions(questions);
	}

	pair<string, int> ReadUsername() {
		string str;
		cout << "Enter username want to ask or -1 to cancel: ";
		if (str == "-1")
			return make_pair("", -1);

		cin >> str;
		if (username_to_user_map.count(str)) {
			User u = username_to_user_map.find(str)->second;
			return make_pair(str, u.getAllowAnonymousQuestions());
		}

		cout << "Invalid username, please try again\n";
		return ReadUsername();
	}
};

class AskMeSystem {
private:
	UsersManger users_manager;
	QuestionsManager questions_manager;

	void LoadDatabase() {
		users_manager.LoadDatabase();
		questions_manager.LoadDatabase();
	}

	void ResetCurrentUserQuestions() {
		const User &user = users_manager.getCurrentUser();

		const auto &from_me_questions = questions_manager.GetFromMeQuestions(user);
		users_manager.RestFromMeQuestions(from_me_questions);
		const auto &to_me_questions = questions_manager.GetToMeQuestions(user);
		users_manager.RestToMeQuestions(to_me_questions);

	}
public:

	void RunSystem() {
		LoadDatabase();
		users_manager.AccessSystem();
		ResetCurrentUserQuestions();

		while(true){
		LoginMenu();

		int choice = ReadInt(1, 8);

		if (choice == 1) {
			users_manager.getCurrentUser().PrintUserInformation();
			questions_manager.PrintToMeQuestions(users_manager.getCurrentUser());
		} else if (choice == 2) {
			questions_manager.PrintFromMeQuestions(users_manager.getCurrentUser());
		} else if (choice == 3) {
			questions_manager.AnswerQuestion(users_manager.getCurrentUser());
		} else if (choice == 4) {
			questions_manager.DeleteQuestion(users_manager.getCurrentUser());
			ResetCurrentUserQuestions();
			questions_manager.UpdateDatabase();

		} else if (choice == 5) {
			pair<string, int> to_user_pair = users_manager.ReadUsername();
			if(to_user_pair.first!="-1"){
				questions_manager.AskQuestion(users_manager.getCurrentUser(), to_user_pair);
				questions_manager.UpdateDatabase();
			}


		} else if (choice == 6) {
			users_manager.ListSystemUsers();

		} else if (choice == 7) {
			questions_manager.ListFeed();

		} else break;


		}
		RunSystem();
	}
	void LoginMenu() {
		vector<string> loginMenu;
		loginMenu.push_back("\n-------- Main Menu --------\n");
		loginMenu.push_back("1.Print Questions To Me");
		loginMenu.push_back("2.Print Questions From Me");
		loginMenu.push_back("3.Answer Question");
		loginMenu.push_back("4.Delete Question ");
		loginMenu.push_back("5.Ask Question");
		loginMenu.push_back("6.List System Users");
		loginMenu.push_back("7.Feed");
		loginMenu.push_back("8.Logout");
		ShowMenu(loginMenu);
	}

};

int main() {

	AskMeSystem ams;
	ams.RunSystem();










	return 0;




}
