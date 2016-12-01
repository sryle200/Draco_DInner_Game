/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();

	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Set filename
	theFile.setFileName("Data/newHighScore.dat");
	// Check file is available
	if (!theFile.openFile(ios::in)) //open file for input output
	{
		cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
		fileAvailable = false;
	}
	else
	{
		cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
		fileAvailable = true;
	}

	theAreaClicked = { 0, 0 };
	score = 0;

	// Store the textures
	textureName = { "meat1", "meat2", "veg1", "veg2", "Egg", "theDragon", "theBackground", "left_buttonsprite", "right_buttonsprite" };
	texturesToUse = { "Images\\Steak Sprite.png", "Images\\Chicken Sprite.png", "Images\\Carrot Sprite.png", "Images\\Broccoli Sprite.png", "Images\\Egg Sprite.png", "Images\\Dragon Sprite.png", "Images\\backgroundImage.png", "Images\\computer_key_Arrow_Left.png", "Images\\computer_key_Arrow_Right.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Create textures for Game Dialogue (text)
	fontList = { "fantasy1", "fantasy2","fantasy1_small" };
	fontsToUse = { "Fonts/Wizards Magic.ttf", "Fonts/A Gothique Time .ttf" };

	theFontMgr->addFont(fontList[0], fontsToUse[0], 50);
	theFontMgr->addFont(fontList[1], fontsToUse[1], 100);
	theFontMgr->addFont(fontList[2], fontsToUse[0], 22);
	
	gameTextList = { "Draco Dinner", "", "Menu" ,"Use the arrow keys to move you dragon left and right to collect the food falling from the sky!","Dont let the food reach the ground or its game over!","Instructions","Game Over"};

	//Assigning Fonts to the Desired text
	theTextureMgr->addTexture("Title", theFontMgr->getFont("fantasy2")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("fantasy2")->createTextTexture(theRenderer, gameTextList[1] , SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Menu Title", theFontMgr->getFont("fantasy1")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions", theFontMgr->getFont("fantasy1_small")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions2", theFontMgr->getFont("fantasy1_small")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Instructions_title", theFontMgr->getFont("fantasy1")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Game_Over", theFontMgr->getFont("fantasy1")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));

	//initialising the score
	/*score = 0;
	theTextureMgr->addTexture("Score", theFontMgr->getFont("fantasy1")->createTextTexture(theRenderer, scoreStr.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));*/


	// Store the Buttons
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	// Load game sounds
	soundList = { "theme", "bite", "death" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio\\BackgroundMusic1.wav", "Audio\\Bite Sound.wav", "Audio\\Player Death Sound.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}
	theSoundMgr->getSnd("theme")->play(-1);

	//Rendering the Background
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//Rendering the Dragon Character
	theDragon.setSpritePos({ 370, 400});
	theDragon.setTexture(theTextureMgr->getTexture("theDragon"));
	theDragon.setSpriteDimensions(theTextureMgr->getTexture("theDragon")->getTWidth(), theTextureMgr->getTexture("theDragon")->getTHeight());
	theDragon.setDragonVelocity({ 0, 0 });
	

	// Create vector array for spawning food
	srand( time(NULL));
	float spawnPointX[] = { 100, 250, 300, 350, 450, 640, 700 };
	float spawnPointY[] = { 40, 40, 40, 40, 40, 40, 40 };
	int randomInt = rand() % 7;

	
		for (int food = 0; food < 5; food++)
		{
			theCollectables.push_back(new cCollectable);
			randomInt = rand() % 7;
			theCollectables[food]->setSpritePos({ spawnPointX[randomInt], spawnPointY[randomInt] });
			theCollectables[food]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
			int randFood = rand() % 4;
			theCollectables[food]->setTexture(theTextureMgr->getTexture(textureName[randFood]));
			theCollectables[food]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randFood])->getTWidth(), theTextureMgr->getTexture(textureName[randFood])->getTHeight());
			theCollectables[food]->setCollectableVelocity({ 3.0f, 3.0f });
			theCollectables[food]->setActive(true);
		}
	
}
	

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();
		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}


void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		//render the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render the Menu Title
		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Menu Title");
		SDL_Rect pos2 = { 20, 120, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);

		//Render the Egg
		cTexture* tempEgg = theTextureMgr->getTexture("Egg");
		SDL_Rect eggPos = { 450, 600, tempEgg->getTextureRect().w, tempEgg->getTextureRect().h };
		FPoint eggScale = { 1, 1 };
		tempEgg->renderTexture(theRenderer, tempEgg->getTexture(), &tempEgg->getTextureRect(), &eggPos, eggScale);

		// Render Buttons
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 200 });
		theButtonMgr->getBtn("instructions_btn")->render(theRenderer, &theButtonMgr->getBtn("instructions_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("instructions_btn")->getSpritePos(), theButtonMgr->getBtn("instructions_btn")->getSpriteScale());
		theButtonMgr->getBtn("instructions_btn")->setSpritePos({ 410, 275 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 450, 400 });
	}
	break;

	case INSTRUCTIONS:
	{
		//render the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render the intruction Title
		cTexture* tempTextTexture4 = theTextureMgr->getTexture("Instructions_title");
		SDL_Rect pos4 = { 20, 120, tempTextTexture4->getTextureRect().w, tempTextTexture4->getTextureRect().h };
		FPoint scale4 = { 1, 1 };
		tempTextTexture4->renderTexture(theRenderer, tempTextTexture4->getTexture(), &tempTextTexture4->getTextureRect(), &pos4, scale4);

		//render the Instruction Sprites
		//Left
		cTexture* tempLeft = theTextureMgr->getTexture("left_buttonsprite");
		SDL_Rect leftPos = { 300, 370, tempLeft->getTextureRect().w, tempLeft->getTextureRect().h };
		FPoint leftScale = { 1, 1 };
		tempLeft->renderTexture(theRenderer, tempLeft->getTexture(), &tempLeft->getTextureRect(), &leftPos, leftScale);
		//Right
		cTexture* tempRight = theTextureMgr->getTexture("right_buttonsprite");
		SDL_Rect rightPos = { 600, 370, tempRight->getTextureRect().w, tempRight->getTextureRect().h };
		FPoint rightScale = { 1, 1 };
		tempRight->renderTexture(theRenderer, tempRight->getTexture(), &tempRight->getTextureRect(), &rightPos, rightScale);

		//render the Instruction writing
		//writing above arrow keys
		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Instructions");
		SDL_Rect pos2 = { 30, 330, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);
		//writing below arrow keys
		cTexture* tempTextTexture3 = theTextureMgr->getTexture("Instructions2");
		SDL_Rect pos3 = { 250, 520, tempTextTexture3->getTextureRect().w, tempTextTexture3->getTextureRect().h };
		FPoint scale3 = { 1, 1 };
		tempTextTexture3->renderTexture(theRenderer, tempTextTexture3->getTexture(), &tempTextTexture3->getTextureRect(), &pos3, scale3);

		//render Buttons
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 200 });
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 450, 250});
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());


	}
	break;

	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each collectable in the vector array
			for (int draw = 0; draw < theCollectables.size(); draw++)
			{
				theCollectables[draw]->render(theRenderer, &theCollectables[draw]->getSpriteDimensions(), &theCollectables[draw]->getSpritePos(), theCollectables[draw]->getSpriteRotAngle(), &theCollectables[draw]->getSpriteCentre(), theCollectables[draw]->getSpriteScale());
			}

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		// Render Buttons
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 700, 700 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		
		//Render the score
		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Score");
		SDL_Rect pos2 = {20, 120, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);


		// render the Dragon
		theDragon.render(theRenderer, &theDragon.getSpriteDimensions(), &theDragon.getSpritePos(), theDragon.getSpriteRotAngle(), &theDragon.getSpriteCentre(), theDragon.getSpriteScale());

	}
	break;

	case LOSE:
	{
		//Render the Background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//Render the game over
		cTexture* tempTextTexture2 = theTextureMgr->getTexture("Game_Over");
		SDL_Rect pos2 = { 20, 120, tempTextTexture2->getTextureRect().w, tempTextTexture2->getTextureRect().h };
		FPoint scale2 = { 1, 1 };
		tempTextTexture2->renderTexture(theRenderer, tempTextTexture2->getTexture(), &tempTextTexture2->getTextureRect(), &pos2, scale2);
		
		//Render the Buttons
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;

	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	// complete the Render
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{
	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// CHeck Button clicked and change state
	if (theGameState == MENU || theGameState == LOSE)
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		}
	else
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, LOSE, theAreaClicked);
		}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	theGameState = theButtonMgr->getBtn("instructions_btn")->update(theGameState, INSTRUCTIONS, theAreaClicked);

	// Update the visibility and position of each collectable if the game state is the playing gamestate
	if (theGameState == PLAYING)
	{
		vector<cCollectable*>::iterator collectableIterator = theCollectables.begin();
		while (collectableIterator != theCollectables.end())
		{
			if ((*collectableIterator)->isActive() == false)
			{
				collectableIterator = theCollectables.erase(collectableIterator);
			}
			else
			{
				(*collectableIterator)->update(deltaTime);
				++collectableIterator;
			}
		}

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		//(*collectableIterartor)->update(deltaTime);
		for (vector<cCollectable*>::iterator collectableIterator = theCollectables.begin(); collectableIterator != theCollectables.end(); ++collectableIterator)
		{
			if ((*collectableIterator)->collidedWith(&(*collectableIterator)->getBoundingRect(), &theDragon.getBoundingRect()))
			{
				// if a collision set the collectables to false
				(*collectableIterator)->setActive(false);
				theSoundMgr->getSnd("bite")->play(0);
				//this updates the score,  turns it into a string and concatenates it to the text in the gameTextList.
				//score += 10;
				//scoreStr = gameTextList[1] + to_string(score);
				////the old score is deleted and gets replaced by the new one
				//theTextureMgr->deleteTexture("Score");
			}
			if ((*collectableIterator)->getSpriteTranslation().y == 800)
			{
				(*collectableIterator)->setActive(false);
				theSoundMgr->getSnd("death")->play(0);
				theGameState = LOSE;

			}
		}

		// Update the Dragons position
		theDragon.update(deltaTime);
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		/*switch (event.type)
		{*/
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}

			break;
			default:
				break;
			}
			break;
	

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				theLoop = false;
				break;
			}
			break;

			case SDLK_RIGHT:
			{
				if (theDragon.getSpritePos().x < (renderWidth - (theDragon.getSpritePos().w + 40)))
				{
					theDragon.setDragonVelocity({ 260.0, 0.0 });
				}
			}
			break;

			case SDLK_LEFT:
			{
				if (theDragon.getSpritePos().x > 0)
				{
					theDragon.setDragonVelocity({ -200.0, 0.0 });
				}
			}
			break;
			default:
				break;
			}
		default:
			break;
		}
	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}
