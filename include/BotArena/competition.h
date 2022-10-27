#pragma once
#ifndef ARENA_STATIC
    #ifdef _WIN32
        #ifdef EXPORTS
            #define ARENA_EXPORT __declspec(dllexport)
        #else
            #define ARENA_EXPORT __declspec(dllimport)
        #endif
    #else
        #define ARENA_EXPORT
    #endif
#else
    #define ARENA_EXPORT
#endif

//Feel free to help me by reporting bugs at https://github.com/Umbe-Exe/BotArena

class Arena_Impl;

enum System {
    MISSILES, LASERS, SHIELDS
};

enum Armament {
    MISSILE, LASER
};

enum BumpType {
    WallHit = 1,
    BotCollision = 2,
    MissileHit = 4,
    LaserHit = 8,
    BlastWave = 16
};

enum COLOR {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    RANDCOL
};

//Coordinates always in range 0..1 and heading angle in degrees 360 
struct GPSdata {
    float x, y, heading;
};

class ARENA_EXPORT Controller {

public:
    /******
        Add a sensor with:
            angle: where to point it (360 degrees)
            width: how wide you want it (360 degrees)
            range: a percentage ofradarMaxRange

        Returns an ID to use with:
            setSensorStatus()
            getSensorData()
    */
    int addRadarGetId(float angle, float width, float range);

    /******
        Add a sensor with:
            angle: where to point it (360 degrees)
            range: a percentage of rangeMaxRange

        Returns an ID to use with:
            setSensorStatus()
            getSensorData()
    */
    int addRangeGetId(float angle, float range);

    /*******
        Each enabled sensor consumes sensorEnergyConsumption units of power over maxEnergy units.
        Sensors are enabled by default.

            enabled: true to enable it, false to disable it.
    */
    void setSensorStatus(int sensorId, bool enabled);

    /*******
        Disabled sensors return -1.

            A radar returns a 1 when a robot is in its area.
            A laser range returns a percentage of how close something is,
                a wall or a bot, from 0 to 100, with 0 being nothing in sight.

    */
    int getSensorData(int sensorId);

    //Values in range -100..100
    void setMotorSpeed(float leftTread, float rightTread);

    /******
        Returns a percentage of the supply of energy, 0 to 100, with 100 being full supply.
    */
    int getGeneratorOutput();

    /******
        Returns the level of the selected system (0 to 100)
    */
    int getSystemEnergy(System system);

    /******
        The sum of the charge rates can't exceed 100, so be careful.
        You don't have to take into account the sensors consumption,
            because it will be spread evenly in any case,
            but you won't be penalized if you do,
            if you know the value of sensorEnergyConsumption and maxEnergy...
    */
    void setSystemChargeRate(System system, float rate);

    //Heading angle in degrees 360
    void fireWeapon(Armament weapon, float heading);

    GPSdata getGPSdata();

    /******
        Check the bits specified in BumpType to know what has bumped into your robot
    */
    char isBumping();

    virtual void onStart() = 0;
    virtual void onUpdate(double delta) = 0;
    virtual void onDeath() = 0;

    virtual const char *name() {
        return "unknown";
    }
    virtual COLOR color() {
        return RANDCOL;
    }
    virtual const char *pathToImage() {
        return 0;
    }

private:
    Arena_Impl *arena;
    friend class Arena_Impl;
};