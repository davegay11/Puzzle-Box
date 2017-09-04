# Puzzle-Box
An independently designed Arduino puzzle box 

During my junior year, I independently designed a wooden box with several embedded sensors including buttons, photocells, an IR rangefinder, a piezo, and an accelerometer. When turned on, the box is locked by a servo attached to the inside of the lid. A user interacts with the box, completes stages based on each sensor, and eventually unlocks the box. To assist the user, there are 5 stage LEDs arranged in a column, a good status LED, a bad status LED, and a vibration motor. When a user successfully completes a step in a stage (for instance, tilts the box the correct direction during the accelerometer stage), the respective stage LED and the good status LED flicker. If a user incorrectly does a step, the bad (red) status LED flashes, the box vibrates, and the stage is reset. This provides continuous feedback, making the box possible, but still difficult to unlock. Once an entire stage is successfully completed, that stage's LED is illuminated and stays on until the box is turned off or unlocked. When all five stages are completed, the box is unlocked.

On average, the box takes users ~45 minutes to unlock with no hints from anybody else. The project was an excellent challenge, allowing me to learn both an incredible amount of new hardware skills, and providing several interesting software problems. I've included the code with some modifications. If you would like to recreate a version of the box, feel free to reach out, and I would be happy to share detailed designs of the hardware.
