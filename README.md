# Multiplayer CoopGame "Course"
#### Engine Version 4.21.0
### A Coop multiplayer game made with ue4.

## What I did in this Project
  * Made the entire process of setting up a character
    * Made Character Class that Drives our player character such as Weapon system, Character movememnt and controls, death and     respawn..etc
      * Character Class includes: Setting up the player capsule.
      * Sprint Arm component for the camera.
      * Health Component to manage death sequence and taking damage.
      * Player input ( walk, run, jump, Zoom in and zoom out, crouch).
      * Handled Weapon spawning.
      * Replicating Some properties so it be MULTIPLAYER as well! and so on...
    * Setting up Abilities in C++ and Blueprints such as:
      * Health Regen.
      * Speed booster.
      * Both abilities are multiplayer ready.
    * Setting Up weapon in C++:
      * Attaching weapon to character.
      * Applying damage based on what part was hit in the body ( head shot, arm ... etc).
      * I use Line tracing to apply damage ( Ray casting ).
      * Simple camera shaking when we shoot.
      * Automatic fire mode.
      * Created two weapons, normal rifle and a Grenade launcher that detonates on impact of after a specified time.
      * Simple Recoil using maths !
      * and it is multiplayer ready as well :)
    * Setting up Advanced Enemy A.I using Behaviour tree:
      * Using Unreal engine's Behaviour tree to make the Enemy A.I Behaviour.
      * Using EQS (Environment Query System) to Find nearest player, Take cover when taking damage, Regenerates its health.
      * Using Unreal Engine's Decorators, Services and tasks to manage the Behaviour Tree.
    * Setting up Simple Tracker bot A.I using C++:
      * It basically like a tracking grenade that finds the players position, store it position then goes to this location when it reaches it searches for the player's position again and so on
      * Detonates when it is near the player dealing Radial Damage that is determined by how close the player(s) is to the TrackerBot.

## Here is a gameplay video

## Here are some Gifs, pics ,Important code snippits and blueprints that shows the highlights of the things I did :)
      
      
    
    
      
