// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"



/*
	Important Notes: GAME MODE
	The GameModeBase defines the game being played. 
	It governs the game rules, scoring, what actors are allowed to exist in this game type, and who may enter the game.
	It is only instanced on the server and will never exist on the client.
*/


/*
	Important Notes: Game State
	Game State holds all of our replicated information about the current game and that is because we can not put that in game mode
	because as we know game mode only exist on the server and clients do not actually have a copy of game mode, 
	that means whenever we want to replicate something we have 
	to put that in a separate class which is GameState.
*/


/*
	Important Notes: Player State
	That contains all of the persistent information of a player and that is because the player controller also does not exist on other clients
	it only exist on YOUR MACHINE and on the server.
*/