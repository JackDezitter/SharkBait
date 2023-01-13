#pragma once
#include <engine.h>
#include <GLFW/include/GLFW/glfw3.h>

class player{
public:
	player();
	~player();
	void init();
	int getHealth() { return health;}
	void setHealth(int health) {
		if (health >= this->health) {
			if (health > 100) {
				this->health = 100;
			}
			this->health = health;
		}
		else if (getShield() <= 0) {
			this->health = health;
		}
		else if (getShield() > 0 && oxygen != 0) {
			int newShield = getShield() - (this->health - health);
			if (newShield < 0) {
				this->health = getHealth() + newShield;
				setShield(0);
			}
			else {
				setShield(newShield);
			}
		}
		else this->health = health;
	}

	int getAmmoCount() { return ammoCount; }
	void setAmmoCount(int ammoCount) { this->ammoCount = ammoCount; }

	int getShield() { return shield; }
	void setShield(int newShield) { shield = newShield; }
	int getOxygen() { return oxygen; }
	void setOxygen(int oxygen) { if (oxygen <= 100 && oxygen > 0)this->oxygen = oxygen; else if (oxygen <= 0) this->oxygen = 0; else this->oxygen = 100; }
	int getOxygenCounter() { return oxygenCounter; }
	void setOxygenCounter() {oxygenCounter = glfwGetTime() + 1; }
	float getDrowningTimer() { return drowningTimer; }
	void setDrowningTimer(float timer) { drowningTimer = timer; }

	bool getBoost() { return boost;}
	void setBoost(bool newBoost) { this->boost = newBoost; }

	float getShotCooldown() { return shotCooldown; }
	void setShotCooldown() { shotCooldown = glfwGetTime()+0.5; }
	void reset() {
		health = 100;
		ammoCount = 30;
		oxygen = 100;
		shield = 0;
		setOxygenCounter();
	}
private:
	int health;
	int ammoCount;
	int shield = 0;
	int oxygen;
	int oxygenCounter;
	float drowningTimer;
	float shotCooldown = 0;
	bool boost;
};
