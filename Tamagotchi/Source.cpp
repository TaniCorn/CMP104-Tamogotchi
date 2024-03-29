//Tamogotchi Pet C++, CMP104 Assignment 2020 Abertay University - Tanapat Somrid 2000891
#pragma region Libraries, Definition, and global variabeles
#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include <fstream>


#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_ENTER 13

#define WDOWNKEY_ENTER 0x0D
#define WDOWNKEY_ESC 0x1D


bool gameQuitted;
int buttonControl = 1;//1 to 4 selection
bool onMainMenu;
time_t previousTime;//Stores last time program opened
#pragma endregion


void DisposeFile();

class Tamogotchi {
public:
	std::string petName;
	int petHappiness = 0;
	int petHunger;
	int petTiredness;
	bool petDead = false;
	int tirednessDownTime;//time to decrease tiredness stat
	int hungerDownTime;//time to decrease hunger stat
	

	void SetPetName() {//I know this is quite useless to be here as the Tamogotchi pet is global, this is purely for the sake of cleanliness as it will only ever be called once
		if (petName.length() <= 0)			//checking to make sure txt file wasn't tampered with
		{
			while (petName.size() > 21 || petName.size() <= 0)
			{
				std::cout << "What is the name of your pet? 21 characters MAX" << std::endl;
				std::getline(std::cin, petName);
			}
		}

	}
	void SetPetStats() {
		srand(time(0));
		petHunger = rand() % 4 + 2;
		petTiredness = rand() % 4 + 2;
	}
	void TamperingFile() {
		if(petName.length() > 21) {
			std::cout << "You've been messing with the txt file haven't you? Naughty naughty.";
			Sleep(5000);
			petName = "Naughty";
		}
	}
	

	void UpdatePetHappiness() {
		//Updates all stats before calulating happiness
		UpdatePetHunger();
		UpdatePetTiredness();
		StatRange();

		petHappiness = ceil((petHunger + petTiredness) / 2);
		if (petHappiness == 1) {
			std::cout << "Your pet has died, commencing garbage disposal" << std::endl;
			Sleep(2000);
			petDead = true;
			gameQuitted = true;
		}
		
	}
	void UpdatePetHunger() {
		//Calculate if hunger should go down, this is compared from last time hunger went down
		int tempTime = difftime(time(0), hungerDownTime);//Time diff from now and last time hungerDownTime = time(0)
		if ((tempTime / 30) >= 1)
		{
			petHunger = petHunger - floor((tempTime / 30));//update how much hunger lost since last update
			hungerDownTime = time(0);
		}

		StatRange();
	}
	void UpdatePetTiredness() {
		//Calculate if tiredness should go down, this is compared from last time tiredness went down
		int tempTime = difftime(time(0), tirednessDownTime);//Time diff from now and last time and tirednessDownTime = time(0)
		if ((tempTime / 60) >= 1)
		{
			petTiredness = petTiredness - floor((tempTime / 60));//update how much tiredness lost since last nap
			tirednessDownTime = time(0);
		}

		StatRange();
	}

	//Screen call functions - trying to nap or feed - if possible allow screen change
	bool AddHunger() {
		UpdatePetHunger();

		int temporaryTime = difftime(time(0), feedTimeElapsed);//delta time since last fed
		if ((temporaryTime / 15) >= 1)//if 15 seconds elapsed, allow feeding
		{
			petHunger++;
			hungerDownTime = time(0);//reset time for decreasing hunger
			feedTimeElapsed = time(0);//feed time resets
			return true;
		}
		else {
			//goes to screen, no can do
			return false;
		}

	}
	bool AddTiredness() {
		UpdatePetTiredness();

		int temporaryTime = difftime(time(0), napTimeElapsed);//delta time since last nap
		if ((temporaryTime / 30) >= 1)//if 30 seconds elapsed, allow nap
		{
			petTiredness++;
			tirednessDownTime = time(0);//reset time for decreasing tiredness
			napTimeElapsed = time(0);//nap time resets
			return true;
		}
		else {
			//goes to screen, no can do
			return false;
		}
	}
private:
	int napTimeElapsed = 0;//time before another nap allowed
	int feedTimeElapsed = 0;//time before another meal allowed

	//Limits hunger and tiredness between 0 and 6
	void StatRange() {
		if (petHunger <= 0) {
			petHunger = 1;
		}
		else if (petHunger >= 6) {
			petHunger = 5;
		}
		if (petTiredness <= 0) {
			petTiredness = 1;
		}
		else if (petTiredness >= 6) {
			petTiredness = 5;
		}
	}
};
Tamogotchi playersPet;

#pragma region Screens
//TODO Allow different pet faces
class MainMenuScreen
{	
public:

	//Shows the player what options they have selected
	std::string SetScreen() {
		switch (buttonControl)
		{
		case 1:
			return face + name + feed;
		case 2:
			return face + name + nap;
		case 3:
			return face + name + stats;
		case 4:
			return face + name + quit;
		default:
			break;
		}
	}
	//Allows the petname to be incorporated into the main menu screen by adding char "-" until edge
	void GenerateSpace() {
		int x = 22 - (playersPet.petName.length());
		std::string nameExtender;
		for (int i = 0; i < x; i++)
		{
			nameExtender += "-";
		};
		nameExtender = nameExtender + "|";
		name = name + playersPet.petName + nameExtender + "\n	|                                                   |\n	|                                                   |";
	}

private:
#pragma region Visuals
	std::string name = "\n	|-----------HENLO, MY NAME IS ";//name x should have a maximum of y characters. for every character in x add 21-x characters of z

	std::string face = R"(
	_____________________________________________________
	|                                                   |
	|   ____________________________________________    |
	|   |    ___________             ___________    |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |___________|           |___________|   |   |
	|   |                                           |   |
	|   |                                           |   |
	|   |                ___________                |   |
	|   |                                           |   |
	|   |                                           |   |
	|   |___________________________________________|   |
	|                                                   |)";

#pragma region Variations
	std::string feed = R"(
	|   ########    OOOOOOO    OOOOOOOOO    OOOOOOOO    |
	|   # FEED #    O NAP O    O STATS O    O QUIT O    |
	|   ########    OOOOOOO    OOOOOOOOO    OOOOOOOO    |
	|                                                   |
	|___________________________________________________|
)";
	std::string nap = R"(
	|   OOOOOOOO    #######    OOOOOOOOO    OOOOOOOO    |
	|   O FEED O    # NAP #    O STATS O    O QUIT O    |
	|   OOOOOOOO    #######    OOOOOOOOO    OOOOOOOO    |
	|                                                   |
	|___________________________________________________|
)";
	std::string stats = R"(
	|   OOOOOOOO    OOOOOOO    #########    OOOOOOOO    |
	|   O FEED O    O NAP O    # STATS #    O QUIT O    |
	|   OOOOOOOO    OOOOOOO    #########    OOOOOOOO    |
	|                                                   |
	|___________________________________________________|
)";
	std::string quit = R"(
	|   OOOOOOOO    OOOOOOO    OOOOOOOOO    ########    |
	|   O FEED O    O NAP O    O STATS O    # QUIT #    |
	|   OOOOOOOO    OOOOOOO    OOOOOOOOO    ########    |
	|                                                   |
	|___________________________________________________|
)";
#pragma endregion
#pragma endregion
};

class StatsMenu
{
public:
	//Shows player stat screen
	std::string SetScreen() {
		SetHunger();
		SetTiredness();
		SetHappiness();
		SetTip();

		return top + hunger1 + hunger2 + space + tiredness1 + tiredness2 + space + happiness1 + happiness2 + space + tip + space + back;
	}

private:
//Strings based on what their respective stats are
	void SetHunger() {
		switch (playersPet.petHunger)
		{
		case 1:
			hunger2 = "\n	|        " + ReplaceName("Your pet is DEAD");
			break;
		case 2:
			hunger2 = "\n	|        " + ReplaceName("Your pet is STARVING");
			break;
		case 3:
			hunger2 = "\n	|        " + ReplaceName("Your pet is RATHER HUNGRY");
			break;
		case 4:
			hunger2 = "\n	|        " + ReplaceName("Your pet is SLIGHTLY PECKISH");
			break;
		case 5:
			hunger2 = "\n	|        " + ReplaceName("Your pet is WELL FED");
			break;
		default:
			break;
		}
	}
	void SetTiredness() {
		switch (playersPet.petTiredness)
		{
		case 1:
			tiredness2 = "\n	|        " + ReplaceName("Your pet is COLLAPSED");
			break;
		case 2:
			tiredness2 = "\n	|        " + ReplaceName("Your pet is FALLING ASLEEP");
			break;
		case 3:
			tiredness2 = "\n	|        " + ReplaceName("Your pet is TIRED");
			break;
		case 4:
			tiredness2 = "\n	|        " + ReplaceName("Your pet is AWAKE");
			break;
		case 5:
			tiredness2 = "\n	|        " + ReplaceName("Your pet is WIDE AWAKE");
			break;
		default:
			break;
		}
	}
	void SetHappiness() {
		switch (playersPet.petHappiness)
		{
		case 1:
			happiness2 = "\n	|        " + ReplaceName("Your pet is DEPRESSED");
			break;
		case 2:
			happiness2 = "\n	|        " + ReplaceName("Your pet is UNHAPPY");
			break;
		case 3:
			happiness2 = "\n	|        " + ReplaceName("Your pet is WORRIED");
			break;
		case 4:
			happiness2 = "\n	|        " + ReplaceName("Your pet is HAPPY");
			break;
		case 5:
			happiness2 = "\n	|        " + ReplaceName("Your pet is ECSTATIC");
			break;
		default:
			break;
		}
	}
	void SetTip() {
		if (playersPet.petDead == true)
		{
			tip = "\n	|        Well, you did your best... right?          |";
		}
		else if (playersPet.petHunger == playersPet.petTiredness) {
			tip = "\n	|   Maybe you should feed then let your pet rest    |";
		}
		else if (playersPet.petHunger <= playersPet.petTiredness) {
			tip = "\n	|   Maybe you should feed your pet? yes yes?        |";
		}
		else {
			tip = "\n	|       Maybe you should let your pet sleep         |";
		}
		
	}

	std::string ReplaceName(std::string message) {
		message.replace(message.find("Your pet"), 8, playersPet.petName);//Replaces Your pet/ just in case I want to change something or something bugs out
		int x = 43 - (message.length());;
		for (int i = 0; i < x; i++)
		{
			message += " ";
		};

		message = message + "|";
		return message;
	}

#pragma region Strings
	std::string top = R"(
	_____________________________________________________
	|                                                   |
	|                                                   |)";

	std::string space = R"(
	|                                                   |)";

	std::string hunger1 = R"(
	|        Hunger:                                    |)";
	std::string hunger2 = "\n	|        X                                          |";

	std::string tiredness1 = R"(
	|        Tiredness:                                 |)";
	std::string tiredness2 = "\n	|        X                                          |";

	std::string happiness1 = R"(
	|        Happiness:                                 |)";
	std::string happiness2 = "\n	|        X                                          |";

	std::string tip = "\n	|        X                                          |";

	std::string back = R"(
	|                     ########                      |
	|                     # BACK #                      |
	|                     ########                      |
	|                                                   |
	|___________________________________________________|
	)";
#pragma endregion
};

class NapMenu {
public:
	std::string SetScreen() {
		if (playersPet.AddTiredness())
		{
			return nap + back;
		}
		else {
			return no + back;
		}
	}

private:
#pragma region Visuals
	std::string no = R"(
	_____________________________________________________
	|                                                   |
	|   ____________________________________________    |
	|   |    ___________             ___________    |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |___________|           |___________|   |   |
	|   |   |          |                        |   |   |
	|   |   |                                   |   |   |
	|   |   |            ___________            |   |   |
	|   |   |                                       |   |
	|   |   |                                       |   |
	|   |___________________________________________|   |
	|                                                   |
	|                 I just woke up                    |
	|                                                   |)";
	std::string nap = R"(
	_____________________________________________________
	|                                                   |
	|                                      _________    |
	|                                              /    |
	|                                            /      |
	|                                          /        |
	|                        ______          /          |
	|                            /         /________    |
	|                          /                        |
	|                        /____                      |
	|                ___                                |
	|                  /                                |
	|                /__                                |
	|                                                   |
	|                                                   |)";

	std::string back = R"(
	|                     ########                      |
	|                     # BACK #                      |
	|                     ########                      |
	|                                                   |
	|___________________________________________________|
	)";
#pragma endregion
};

class FeedMenu {
public:
	std::string SetScreen() {
		if (playersPet.AddHunger())
		{
			return food + back;
		}
		else {
			return no + back;
		}
	}

private:
#pragma region Visuals
	std::string no = R"(
	_____________________________________________________
	|                                                   |
	|   ____________________________________________    |
	|   |    ___________             ___________    |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |           |           |           |   |   |
	|   |   |___________|           |___________|   |   |
	|   |   |          |                        |   |   |
	|   |   |                                   |   |   |
	|   |   |            ___________            |   |   |
	|   |   |                                       |   |
	|   |   |                                       |   |
	|   |___________________________________________|   |
	|                                                   |
	|                Too full right now                 |
	|                                                   |)";
	std::string food = R"(
	_____________________________________________________
	|                                                   |
	|                  It's a carrot                    |
	|                                                   |
	|                    _  /\  _                       |
	|                    \\/  \//                       |
	|                     \____/                        |
	|                     |    |                        |
	|                     |    |                        |
	|                     |    |                        |
	|                     |    |                        |
	|                      \  /                         |
	|                       \/                          |
	|                                                   |
	|                                                   |)";

	std::string back = R"(
	|                     ########                      |
	|                     # BACK #                      |
	|                     ########                      |
	|                                                   |
	|___________________________________________________|
	)";
#pragma endregion
};

#pragma endregion
MainMenuScreen mainMenu;


#pragma region ProperFunctions
void ButtonControlRange() {
	if (buttonControl == 0) {
		buttonControl = 4;
	}
	else if (buttonControl == 5) {
		buttonControl = 1;
	}
}

//Changes screens from main menu to others
std::string onScreenChanges() {
	StatsMenu statsMenu;
	FeedMenu feedMenu;
	NapMenu napMenu;
	//returns strings of the screen player has selected from the main menu and updates some/all stats
	if (buttonControl == 1)
	{
		return feedMenu.SetScreen();

	}
	else if (buttonControl == 2)
	{

		return napMenu.SetScreen();

	}
	else if (buttonControl == 3)
	{
		playersPet.UpdatePetHappiness();//Updates all stats
		return statsMenu.SetScreen();
	}
	else if (buttonControl == 4)
	{
		gameQuitted = true;
		return mainMenu.SetScreen();
	}
}

//File, Load/Save Management //TODO:Make save system with multiple pets
void LoadVariablesFromFile(std::string*);
bool LoadFile() {
	std::ifstream openSave("pet1.txt");//open file
		std::string line;
		std::string lines[6];//All information in here
		int i = 0;

//File reading system
	if (openSave.is_open()) {//just to make sure file can be opened
		while (std::getline(openSave, line))//gets all lines in file until line is empty
		{
			lines[i] = line;
			i++;
		}
		LoadVariablesFromFile(lines);
		openSave.close();
		return true;
	}
	else {
		//no save file, start new game
		return false;
	}
}
//DONE: Figure out how to point to array//Done, old vs. (std::string[]), LoadVariablesFromFile(lines), (std::string data[]) 
void LoadVariablesFromFile(std::string* data) {
	playersPet.petName = data[0];
	playersPet.petTiredness = std::stoi(data[1], 0, 10);
	playersPet.petHunger = std::stoi(data[2], 0, 10);
	previousTime = std::stoi(data[3], 0, 10);
	playersPet.tirednessDownTime = std::stoi(data[4], 0, 10);
	playersPet.hungerDownTime = std::stoi(data[5], 0, 10);
	playersPet.TamperingFile();
}
void WriteFile() {
	std::ofstream save("pet1.txt");//create/overwrite file

//File writing system
	if (save.is_open()) {//just to make sure the file can be opened and edited
		save << playersPet.petName << std::endl;
		save << playersPet.petTiredness << std::endl;
		save << playersPet.petHunger << std::endl;
		save << time(0) << std::endl;
		save << playersPet.tirednessDownTime << std::endl;
		save << playersPet.hungerDownTime << std::endl;
	}
	else {
		//file was not openable?
		std::cout << "File got renamed or moved during running of program, maybe?";
	}
	save.close();
}
void DisposeFile() {
	if (remove("pet1.txt")) {
		//File got deleted successfully
		gameQuitted = true;
	}
	else {
		//file was not deletable
		std::cout << "File got renamed or moved during running of program, maybe?";
	}
}

//This will run at start of loading to check difference from time offline to now.

#pragma endregion


void Start() {
	gameQuitted = false;
	if (LoadFile()) {//If there is a save, start game with save
		playersPet.UpdatePetHunger();//updates it from last time(in other program runs)
		playersPet.UpdatePetTiredness();
	}
	else {//Else start new game
		playersPet.SetPetName();
		playersPet.SetPetStats();
		WriteFile();
	}
	
	mainMenu.GenerateSpace();
	onMainMenu = true;
}

void Output() {
	system("cls");//Allows only one image on console at a time, clears console

	if (onMainMenu)
	{//If not changing from main menu show different main menu screen
		std::cout << mainMenu.SetScreen();
	}
	else {//Goes to selected screen
		std::cout << onScreenChanges();
	}

}

void Input() {
#pragma region InputDetection

	//_getwch will pause the entire program, this is fine for my game as I do not have animations or anything that requires the program to loop constantly
	if (onMainMenu && gameQuitted == false) {
		switch (_getwch()) {
		case KEY_RIGHT:
			buttonControl++;
			break;
		case KEY_LEFT:
			buttonControl--;
			break;
		case KEY_ESC:
			gameQuitted = true;
			break;
		case KEY_ENTER://When on main menu and enter pressed. It turns onMainMen off to change screens
			onMainMenu = false;
			break;
		default:
			break;
		}
	}
	else if (gameQuitted == false) {
		switch (_getwch()) {
		case KEY_ENTER://When on any other screen, enter will make screen go back to main menu
			onMainMenu = true;
			break;
		case KEY_ESC:
			gameQuitted = true;
			break;
		default:
			break;
		}
	}

#pragma endregion
	ButtonControlRange();
}

int main() {

	Start();
	playersPet.hungerDownTime = time(0);
	playersPet.tirednessDownTime = time(0);

	//Game Loop
	while (!gameQuitted)
	{
		Output();
		Input();

	}
	if (playersPet.petDead == true)
	{
		DisposeFile();
	}
	else {
		WriteFile();
	}

	return 0;
}