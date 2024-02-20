#include "Controller.h"
#include <cmath>
#include <iostream>
#include <limits>
namespace mrsd
{
	// Task-4:
	void Controller::control(const mrsd::Game& g, float t)
	{
		// can test your functions by determining safe spots, picking one, and instantly moving your player there
		// Clear the existing safe spots
        safeSpots.clear();

        // Determine safe spots based on the updated danger zone
        determineSafeSpots(g);

        // Access players from the game:
        const std::vector<Player*>& players = g.getPlayers();

        // Move each player to the nearest safe spot with a specified speed
        for (Player* playerPtr : players)
        {
            if (playerPtr != nullptr) 
            {
                // Find the nearest safe spot for the current player
				int nearestSafeSpot = -1;
    			int minDistance = std::numeric_limits<int>::max();

				for (int spot : safeSpots) {
					int distance = std::abs(playerPtr->x - spot);
					if (distance < minDistance) {
						minDistance = distance;
						nearestSafeSpot = spot;
					}
				}
                int safeSpot = nearestSafeSpot;

                if (safeSpot != -1) 
                {
                    // Calculate the direction to the safe spot
                    float direction=0.0f;
					if (safeSpot > playerPtr->x)
					{
						direction = 1.0f;
					}
					else if (safeSpot < playerPtr-> x)
					{
						direction = -1.0f;
					}
					
					// std::cout<<"\ndirection:"<<direction;
					std::cout<<"\nsafe spot"<<safeSpot<<"player position"<<playerPtr->x;

                    // // Adjust the player's speed using the game's playerSpeed property
                    // float playerSpeed = g.playerSpeed; // Use the playerSpeed from the Game object

					// Calculate the distance to the safe spot
					float distanceToSafeSpot = std::abs(playerPtr->x - safeSpot);

					// Adjust the player's speed based on the distance to the safe spot
					float maxSpeed = g.playerSpeed;
					float playerSpeed = std::min(maxSpeed, distanceToSafeSpot / t);

                    // Adjust the player's x-coordinate based on direction and player speed
                    playerPtr->x += direction * playerSpeed;

                    // Ensure the player stays within the game bounds
                    playerPtr->x = std::max(0.0f, std::min(static_cast<float>(g.getWidth()), playerPtr->x));
                } 
                else 
                {
                    std::cerr << "Error: No safe spot found." << std::endl;
                }
            } 
            else 
            {
                std::cerr << "Error: Player object is null." << std::endl;
            }
        }

        // Output for debugging
        // std::cout << "Players moved to safe spots." << std::endl;
	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2;
			g.newPlayer(p);
		}
	}

	// Task-1:
	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		Prediction pred;
		// Calculate time of impact using projectile motion equations
		// Use the projectile's initial position, velocity, and g constant

		float discriminant = p.vy*p.vy - 4*0.5f*g.getGravity()*p.y;

		if(discriminant>=0)
		{
			// Calculate the roots
			float root1 = (-p.vy + sqrt(discriminant)) / (2 * 0.5f * g.getGravity());
			float root2 = (-p.vy - sqrt(discriminant)) / (2 * 0.5f * g.getGravity());

			// Choose the positive root (the negative root represents a past collision)
			float t = (root1 >= 0) ? root1 : root2;

			// Populate the prediction state with time and position values
			pred.t = t;
			pred.x = p.x + p.vx * t;
		}
		else
		{
			 // If discriminant is negative, no real roots exist (no impact)
			pred.t = -1; // Set time to -1 to indicate no impact
			pred.x = -1; // Set position to -1 to indicate no impact
		}


		// return impact point: time and position
		return pred;
	}

	// Task-2:
	void Controller::determineSafeSpots(const Game& g)
	{
		/*Disgusting code
		 // Clear the existing safe spots
		safeSpots.clear();

		// Iterate through the ground positions
		for (int i = 0; i <= g.getWidth(); ++i) {
			// Track the projectile for the current position
			bool isSafe = true;
			
			// Iterate through all projectiles to check for explosion range
			for (const auto& projectile : g.getProjectiles()) {
				// Predict the impact point of the projectile at the current ground position
				Prediction pred = trackProjectile({projectile.x, projectile.y, projectile.vx, projectile.vy}, g);

				// Check if the predicted impact point is within the current ground position
				if (std::abs(pred.x - i) <= g.explosionSize && pred.t<=2) {
					isSafe = false;
					break;  // No need to check other projectiles, as this spot is unsafe
				}
			}

			// If the current position is safe, add it to the list of safe spots
			if (isSafe) {
				safeSpots.push_back(i);
			}

		}
		*/
		// Clear the existing safe spots
		safeSpots.clear();

		// Iterate through the ground positions
		for (int i = 0; i <= g.getWidth(); ++i) {
			
			// Track the projectile for the current position
			bool isSafe = true;

			// Iterate through all projectiles to check for explosion range
			std::list<Projectile> projectiles = g.getProjectiles();
			for (const auto& projectile : projectiles) {
				// Predict the impact point of the projectile at the current ground position
				// Prediction pred = trackProjectile({projectile.x, projectile.y, projectile.vx, projectile.vy}, g);
				Prediction pred = trackProjectile(projectile, g);

				// Calculate the distance between the predicted impact point and the current ground position
				int distance = std::abs(pred.x - i);

				// Check if the predicted impact point is within the area of explosion and if the predicted impact time is less than or equal to 2
				if (distance >=0 && distance <= g.explosionSize && pred.t <= 5) {
					isSafe = false;
					break;  // No need to check other projectiles, as this spot is unsafe
				}
			}

			// If the current position is safe, add it to the list of safe spots
			if (isSafe) {
				safeSpots.push_back(i);
			}
		}
	}

	// Task-3:
	int Controller::pickSafeSpot(const Game& g)
	{
		// picks a spot from the determined spots and returns that.
		
		// Call determineSafeSpots to populate the safeSpots list
		determineSafeSpots(g);

		// Iterate through the stored safe spots
		for (int spot : safeSpots) {
			// Return the first safe spot found
			return spot;
		}

		// If no safe spot is found, return a default value
		return -1;

	}
}
