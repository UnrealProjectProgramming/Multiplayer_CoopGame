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
    * Setting up Abilities such as:
      * Health Regen.
      * Speed booster.
      * Both abilities are multiplayer ready.
    * Setting Up weapon:
      * Attaching weapon to character.
      * Applying damage based on what part was hit in the body ( head shot, arm ... etc).
      * I use Line tracing to apply damage ( Ray casting ).
      * Simple camera shaking when we shoot.
      * Automatic fire mode.
      * and it is multiplayer ready as well :)
    
      
