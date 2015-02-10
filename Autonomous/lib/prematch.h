// BUTTONS: -1 (none), 0 (grey), 1 (right), 2 (left), 3 (orange)

int chooseStrategy()
{
	string displayStrategies[] = {
		"Strategy?",
		" 1: None",
		" 2: Points",
		" 3: Defend"
	};
	short currentSelectionStrat = 1;
	//Initialize the display and currentSelection inside the function so it doesn't take up space later

	eraseDisplay();
	for(int i=0; i < 4; i++)	//Clear screen, then display the "strategies" screen
	{
		nxtDisplayString(i, "%s", displayStrategies[i]);
	}

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1 && currentSelectionStrat < 3)	//if right button, go down
		{
			currentSelectionStrat++;
			nxtDisplayString(currentSelectionStrat - 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionStrat, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		else if(nNxtButtonPressed == 2 && currentSelectionStrat > 1)	//if left button, go up
		{
			currentSelectionStrat--;
			nxtDisplayString(currentSelectionStrat + 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionStrat, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		wait1Msec(10);
	}
	return currentSelectionStrat;	//Return the number the person selected (corresponding to screen)
}

int chooseStart()
{
	string displayStart[] = {
		"Start Point?",
		" 1: Platform",
		" 2: Parking Zone"
	};
	short currentSelectionStart = 1;
	//Initialize the display and currentSelection inside the function so it doesn't take up space later

	eraseDisplay();
	for(int i=0; i < 3; i++)	//Clear screen, then display the "start" screen
	{
		nxtDisplayString(i, "%s", displayStart[i]);
	}

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1 && currentSelectionStart < 2)	//if right button, go down
		{
			currentSelectionStart++;
			nxtDisplayString(currentSelectionStart - 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionStart, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		else if(nNxtButtonPressed == 2 && currentSelectionStart > 1)	//if left button, go up
		{
			currentSelectionStart--;
			nxtDisplayString(currentSelectionStart + 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionStart, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		wait1Msec(10);
	}
	return currentSelectionStart;	//Return the number the person selected (corresponding to screen)
}

int chooseGoals()
{
	string displayGoals[] = {
		"Which Goals?",
		" 1: 60cm",
		" 2: 30cm",
		" 3: Both",
		" 4: None"
	};
	short currentSelectionGoals = 1;
	//Initialize the display and currentSelection inside the function so it doesn't take up space later

	eraseDisplay();
	for(int i=0; i < 5; i++)	//Clear screen, then display the "goals" screen
	{
		nxtDisplayString(i, "%s", displayGoals[i]);
	}

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1 && currentSelectionGoals < 4)	//if right button, go down
		{
			currentSelectionGoals++;
			nxtDisplayString(currentSelectionGoals - 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionGoals, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		else if(nNxtButtonPressed == 2 && currentSelectionGoals > 1)	//if left button, go up
		{
			currentSelectionGoals--;
			nxtDisplayString(currentSelectionGoals + 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionGoals, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		wait1Msec(10);
	}
	return currentSelectionGoals;	//Return the number the person selected (corresponding to screen)
}

int chooseDelays()
{
	string displayDelay[] = {
		"Delay?",
		" 1: 0 sec",
		" 2: 1 sec",
		" 3: 4 sec",
		" 4: 9 sec",
		" 5: 16 sec"
	};
	short currentSelectionDelays = 1;
	//Initialize the display and currentSelection inside the function so it doesn't take up space later

	eraseDisplay();
	for(int i=0; i < 6; i++)	//Clear screen, then display the "delays" screen
	{
		nxtDisplayString(i, "%s", displayDelays[i]);
	}

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1 && currentSelectionDelays < 5)	//if right button, go down
		{
			currentSelectionDelays++;
			nxtDisplayString(currentSelectionDelays - 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionDelays, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		else if(nNxtButtonPressed == 2 && currentSelectionDelays > 1)	//if left button, go up
		{
			currentSelectionDelays--;
			nxtDisplayString(currentSelectionDelays + 1, " ");	//Erase the ">" in the last line
			nxtDisplayString(currentSelectionDelays, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		wait1Msec(10);
	}
	return (currentSelectionDelays - 1) * currentSelectionDelays;	//Return delay before starting (regarding the selected number)
}

bool isTestRun()
{
	string displayTestRun[] = {
		"Test Run?",
		" 1: No",
		" 2: Yes"
	};
	bool currentSelection = false;
	//Initialize the display and currentSelection inside the function so it doesn't take up space later

	eraseDisplay();
	for(int i=0; i < 6; i++)	//Clear screen, then display the "delays" screen
	{
		nxtDisplayString(i, "%s", displayTestRun[i]);
	}

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1 && currentSelection == false)	//if right button, go down
		{
			currentSelectionDelays != currentSelection;
			nxtDisplayString(1, " ");	//Erase the ">" in the last line
			nxtDisplayString(2, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		else if(nNxtButtonPressed == 2 && currentSelection == true)	//if left button, go up
		{
			currentSelectionDelays--;
			nxtDisplayString(2, " ");	//Erase the ">" in the last line
			nxtDisplayString(1, ">");	//Add the ">" in the current line
			wait1Msec(250);
		}
		wait1Msec(10);
	}
	return currentSelection;	//Return true/false depending on what's selected
}
