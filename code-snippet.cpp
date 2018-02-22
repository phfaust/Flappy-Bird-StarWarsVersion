		//outside while loop, instantiate these

		int launch = 0;
		sf::Vector2f darthVelocity = {0.f, 0.f};
		float launchOffset = -40.f;
		float fallingOffset = 0.f;

		//change movemeent
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button)
				{
					case sf::Mouse::Left:
						launch = -1;
						break;
					case sf::Mouse::Right:
						launch = 1;
						//cout << myFlags << endl;
						break;
				}
				break;

		//add after change of movement

		if(launch != 0)
		{
			darthVelocity = {30.f * launch, launchOffset};
			launchOffset += 10.f;
		}
		else
		{
			darthVelocity = {darthVelocity.x, fallingOffset};
			fallingOffset += 5.f;
		}

		if(launchOffset == 0.f)
		{
			launch == 0;
			launchOffset = -40.f;
		}

		vader.move(darthVelocity * TIMESTEP);
		maul.move(darthVelocity * TIMESTEP);

		sf::Texture backgroundText;
		if(!background.loadFromFile("Background.png"))
		{}

		sf::Sprite background(backgroundText);


		//UPGRADES



		

		if(powerup != 0)
		{

			if(!initiated)
			{
				powerupClock = elapsedTime;
				initiated = true;
			}
			if(elapsedTime >= powerupClock + 10)
			{
				powerup = 0;
				initiated = false;
			}
		}

		if(elapsedTime % 15 == 0)
		{
			if(charges < 2)
				charges++;
		}