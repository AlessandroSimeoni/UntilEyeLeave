# Until Eye Leave
![Until Eye Leave Promo Art](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/UntilEyeLeave_PromoArt.png)  

Venture in an unknown world to find your way home, beware of the strange creatures that populate this place and learn how to walk among them to solve puzzles.

The player is a cat that is unfortunately passing on to a better life, we, as a player, experience her journey in the afterlife inside her conscience.

Until Eye Leave is a puzzle action-adventure vertical slice developed in Unreal Engine 5.4 for PC.

This is the last, biggest and most important project of my academic studies.
It was developed during 6 months in a pretty big team of approximately 20 people (10 designers, 3 programmers, 4 concept artists and 3 3D artists).
This project has allowed me to learn different features of Unreal Engine, organize the work among us programmers visualizing it in a Gantt view on github, create technical documentation of the code and communicate effectively between departments in a larger than usual team.  

### ***Watch the trailer [here](https://www.youtube.com/watch?v=eUA_2rIrMpU).***
### ***Download the build [here](https://github.com/AlessandroSimeoni/UntilEyeLeave/releases/tag/ReleaseBuild).***

<a name="What-I-did"></a>
# What I did
For this project, I contributed to several core systems.  
I developed a custom finite state machine (FSM) and designed almost all the main character logic around it.  
I also worked on gameplay objects, the save system, the cutscene system, game settings, level loading logic, and various other supporting features.
> 1. [FSM](#FSM)
> 2. [Character](#Character)
> 3. [Gameplay objects](#Gameplay-objects)
> 4. [Save system](#Save-system)
> 5. [Cutscene system](#Cutscene-system)
> 6. [Level loading](#Level-loading)

<a name="FSM"></a>
## 1. FSM
The project contains a custom finite state machine consisting of two basic scripts.  
You can find them here: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/FSM) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Private/FSM).  
There is a StateBase and a StateController.

<a name="State-Base"></a>
### 1.1 State Base
It's the base class of any state of the FSM.  

Each state has 5 public methods:
* **InitState**: called by the *StateController* to initialize the state variables;
* **EnterState**: called by the *StateController* when entering the state;
* **TickState**: called every frame by the *StateController* if the state can tick;
* **ExitState**: called by the *StateController* when exiting the state;
* **IsStateReady**: this returns the *StateReady* protected boolean;

The class also contains a **protected** method called **StateTransition**; this is used to request a change of state to the *StateController*, passing a *TargetState* as parameter (each state must cache references to the states which it is connected to).  

As for parameters, the class contains:
* **CanTick**: a bool which controls the *TickState* method, if **TRUE** the state can tick, otherwise no;
* **StateController**: each state has a reference to the *StateController* and it is initialized in the *InitState* method;
* **StateReady**: a bool used to know if the state is ready to be entered or not; the *StateController* checks this value when it receives a request to change state and, if the target state is not ready, the state transition will fail (this is useful for states with a cooldown effect).

### 1.2 State Controller
The state controller is the brain of the FSM.  
It is initialized at the *BeginPlay* with the **Init** method.  

What the **Init** method does is to create and initialize every state that must be controlled.  
The states under its control are listed in the *AvailableStateClasses* array of `TSubclassOf<class UStateBase>` which is populated in the unreal editor (the first state of this array will be the default state).  
The *Init* method creates `UStateBase` objects based on the classes in the array and saves them in the *States* array (which is a `TArray<UStateBase*>`); each state then is initialized by calling the state’s *InitState* method.  
Finally, the first state in the *AvailableStateClasses* array is entered (this is the default state).  
The core method of the state controller is the **ChangeState** method.  
It accepts one input parameter, the *TargetState*.  
It ensures that the *TargetState* is valid and ready before performing the transition.  
When changing state the *CurrentState* variable is updated together with the *PreviousState* and the *NextState* variables (these two are useful so that the states know which state they are coming from or to which they need to make the transition).  
If *TargetState* is the same as *CurrentState* the **ChangeState** method returns false to prevent unnecessary state changes.  
If the transition can occur, the method calls the *ExitState* of the *CurrentState* and the *EnterState* of the *TargetState* and updates the variables.  

Another important method is the **FindStateOfClass**.  
This is used by the states to get a reference to the state object they request (the method accepts a `TSubclassOf<UStateBase>` as input parameter), so that they can cache references to the states they are connected to.  

The *StateController* is an actor component and to tick the *TickComponent* method is used.  
This method will call the *TickState* of the *CurrentState* if the *StateController* can tick (*CanTickState* variable) and if the *CurrentState* can also tick (*CanTick* variable of the state).  

**[⬆ Back to Top](#What-I-did)**

<a name="Character"></a>
## 2. Character
You can find the scripts related to the character here: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/Character) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Private/Character).  
The scripts include a *CharacterController*, a *CharacterAnimInstance* and of course the *MainCharacter* script along with all the states scripts.  
The character was one of the first elements developed to allow the level designers to test their levels as early as possible.  
When I created the character I didn't know about the Unreal's Gameplay Ability System (GAS), and I had already prototyped the [FSM](#FSM), so since I had already tested it and didn't have enough time to quickly learn GAS, I decided to continue using it.  
Despite this, the character still incorporates functionalities built on top of GAS, added at a later time by a colleague who also implemented all the creatures in the project using the same system.  
If I had to recreate the character today, I would definitely use GAS thanks to its high flexibility.  

### 2.1 Main scripts
The *MainCharacter* script handles the interaction logic, the change of the rotation rate based on the device in use (gamepad/keyboard), the ledge grab detection logic and the life system of the player.  
The *CharacterController* script has the core logic and delegates to handle the user inputs.  
The *CharacterAnimInstance* in the *NativeUpdateAnimation* method, the *IdleToRunRatio* parameter which handles the blend from idle to run animation.  

### 2.2 Character FSM
The FSM is composed of *PlayerStateBase* states which derive from the [StateBase](#State-Base).  
The main difference is that it has references to the *MainCharacter*, the *CharacterController* and the *CharacterAnimInstance*.  

There are a total of 10 scripts for the states:
* [**PlayerIdleState**](#Idle-State)
* [**PlayerStandardMoveState**](#Standard-Move-State)
* [**PlayerSlowMoveState**](#Slow-Move-State)
* [**PlayerMidAirState**](#Mid-Air-State)
* [**PlayerLedgeFallingState**](#Ledge-Falling-State)
* [**PlayerJumpState**](#Jump-State)
* [**PlayerLedgeGrabState**](#Ledge-Grab-State)
* [**PlayerRollState**](#Roll-State)
* [**PlayerKnockbackState**](#Knockback-State)
* [**PlayerCombatState**](#Combat-State)

Each state has references to the states they are directly connected to. These references are initialized in the *InitState* method.  
As a general rule the *EnterState/ExitState* method is used to subscribe/unsubscribe to the events of the *MainCharacter* and/or the *CharacterController* so that they know how to behave and towards which state call the transition.  
Let's take a quick look at them anyway.  


<a name="Idle-State"></a>
### 2.3 Idle State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerIdleState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerIdleState.cpp).  
Pretty simple, the character is in idle animation and the state is listening for the events of movement, attack, jump, roll or knockback if the player is hit by something.

<a name="Standard-Move-State"></a>
### 2.4 Standard Move State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerStandardMoveState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerStandardMoveState.cpp).  
The character moves at a specified speed based on the input direction.  
The character rotates with a rotation rate which is based on the device in use (faster control with a gamepad, slower one with the keyboard).  
There is also a logic that handles an abrupt change in the direction (ie: the character is running left but suddenly the user presses the right input direction); in this case the player rotates towards the direction and stops for a brief moment before moving again.  
All this is done in the *AddMovementDirection* method.
In the *TickState* method there are some checks to handle cases such as: falling, stop moving, slow movement or the ledge grab of low steps.

<a name="Slow-Move-State"></a>
### 2.5 Slow Move State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerSlowMoveState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerSlowMoveState.cpp).  
It’s the same as the standard one but has a different movement speed.

<a name="Mid-Air-State"></a>
### 2.6 Mid Air State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerMidAirState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerMidAirState.cpp).  
This is the base state for all the mid air states (jump and falling).  
Pretty simple, it has checks to go in the roll, idle or ledge grab states; it also handles the mid air movement which is limited compared to the *StandardMoveState*.

<a name="Ledge-Falling-State"></a>
### 2.7 Ledge Falling State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerLedgeFallingState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerLedgeFallingState.cpp).  
A mid air state which handles the coyote time to simplify jumps from ledges.

<a name="Jump-State"></a>
### 2.8 Jump State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerJumpState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerJumpState.cpp).  
Makes the character jump based on the *Jump* method of Unreal.  
Handles the forward and horizontal movement of the player separately.

<a name="Ledge-Grab-State"></a>
### 2.9 Ledge Grab State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerLedgeGrabState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerLedgeGrabState.cpp).  
Handles the ledge grab in air and on the ground.  
The core logic is made in blueprint because it was faster to test it, but it can be easily replicated in C++.

<a name="Roll-State"></a>
### 2.10 Roll State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerRollState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerRollState.cpp).  
The player can roll and he can do it in the direction the user puts in input (instant rotation of the player when entering the state).  
The roll can be interrupted with a jump.  
There is a logic in the *TryEndRolling* method which handles the case where the player is under some object while rolling. This is done to avoid getting stuck in things.  
The logic is kinda simple, it casts a sphere above the player to know if there is something and as long as it returns true, the player remains in this state and it is forcefully moved with a certain speed in a calculated direction.  
There is also a logic that handles the case where it encounters a wall while rolling under an object.  
The logic is not perfect since there are rare cases where the player can go out of map if the colliders are placed in a messy way.

<a name="Knockback-State"></a>
### 2.11 Knockback State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerKnockbackState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerKnockbackState.cpp).  
The knockback was made with GAS by a collegue.
When entering this state the ability is activated from the blueprint version of this state.

<a name="Combat-State"></a>
### 2.12 Combat State
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/Character/PlayerStates/PlayerCombatState.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/Character/PlayerStates/PlayerCombatState.cpp).  
Player can attack in mid air and on ground.
The attack combo and animations were handled with GAS in blueprint by a collegue.
The combat state can be interrupted with a roll.  

**[⬆ Back to Top](#What-I-did)**

<a name="Gameplay-objects"></a>
## 3. Gameplay objects
Gameplay objects are divided in actionable and non-actionable.

### 3.1 Actionable Actor System
The system to control an actor’s behaviour by interacting with another actor is based on two scripts: 
* **ActionableActor**
* **ActuatorActor**

The majority of the gameplay objects in the game (buttons, doors, pressure plates, ….) are based on these two scripts.  
You can find all the actionable gameplay objects here: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/ActionableActors) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Private/ActionableActors).  

#### 3.1.1 Actionable Actor
This is the base script for an actor that can be activated.
It’s very simple, it has one *ActivateActor*/*DeActivateActor* BlueprintNativeEvent that every actor must implement and the methods *RequestActivation*/*RequestDeActivation* that an actuator actor will call when interacted.  
Each actionable has a number of *ActivationRequestsRequired* in order to activate its own behaviour.

#### 3.1.2 Actuator Actor
If there is an actionable actor, there must be an actuator somewhere.  
This is the basic script for an actor that commands one or more actionable actors.  
It contains an array of actionable actors and has a *ActionActors* method that calls the *RequestActivation* method of the connected actors and one *DeActivateActors* method that calls the *RequestDeActivation* method.  

### 3.2 Other Gameplay Objects
#### 3.2.1 Checkpoint
The checkpoint is an interactable actor that serves as a respawn/teleport point during the game. 
It consists of three trigger areas: a save one, an heal one and an interaction one.  
When entering the save area, the game will be saved.  
When entering the heal area, the player will be fully healed.  
When getting close to the interaction area the player can interact to sit on the bench and teleport to another unlocked checkpoint.  
Each checkpoint has a UniqueID component necessary for the save system to know if it has already been discovered and to update the last encountered checkpoint location.  
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/WorldActors/Checkpoint.h) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/WorldActors/Checkpoint.cpp).

#### 3.2.2 Deathzone
The deathzone is an actor with a trigger area; when the player enters the area, gets damaged and teleported at the last grounded location.  
The *OnDeathAreaBeginOverlap* method contains the logic for the teleport and repositioning of the player.  
Once the player is repositioned, the controls are disabled for half a second to avoid unwanted movement.  
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/WorldActors/Deathzone.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/WorldActors/Deathzone.cpp).

#### 3.2.3 Street Lamp
The street lamp is an actor that is related to the CageBird creature.  
At the beginplay it spawns the bird and if it’s hit by the mouse creature lets the bird fall.  
It has a unique ID component used to save its state.  
Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/WorldActors/StreetLamp.h) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/WorldActors/StreetLamp.cpp).

**[⬆ Back to Top](#What-I-did)**

<a name="Save-system"></a>
## 4. Save system
The save system consists of:
* a subsystem containing the core logic
* a `USaveGame` class
* a `USaveSettings` class
* a component for unique IDs

[Scripts](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/SaveSystem)

### 4.1 Save Subsystem
This subsystem contains the core logic to save, load and delete the save file of the game.  
It derives from `UGameInstanceSubsystem` so that it is easily accessible from other scripts or blueprints.  

Functions:
* **SaveGame**: saves the game in a default slot and broadcasts the OnSaveComplete delegate; if the file does not exist, it creates one.
* **LoadGame**: loads the game save file and broadcasts the OnLoadComplete delegate; if the file does not exist, it creates one and returns the save file class.
* **SaveSettings**: saves the game settings in a default slot; if the file does not exist, it creates one.
* **LoadSettings**: loads the game settings file.
* **DeleteSave**: delete the existing save file.

### 4.2 TBOSaveGame
This class derives from `USaveGame`.  
This is what will be saved in the game **save** file.  
It contains the parameters that must be saved and some functions useful to facilitate the saving of some of them.

### 4.3 TBOSaveSettings
This class derives from `USaveGame`.  
This is what will be saved in the game **settings** save file.  
It contains the settings parameters (audio volumes in this case)

### 4.4 UniqueIDComponent
This is an actor component which can be attached to actors that require a **unique ID**.  
The ID is created in the *OnComponentCreated* method.  
This is useful for every actor that must save its state.

**[⬆ Back to Top](#What-I-did)**

<a name="Cutscene-system"></a>
## 5. Cutscene system

Scripts: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/GISubsystem/CutsceneSubsystem.h) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/GISubsystem/CutsceneSubsystem.cpp).  

This system plays the cutscenes made with the level sequencer.  
The main function is the *PlayCutscene*, which has the following input parameters:
* The cutscene sublevel (each cutscene has its elements in a specific sublevel which is loaded at runtime when needed);
* The cutscene level sequence.

The *PlayCutscene* function is a latent function so that in blueprint is easier to handle the execution flow without additional events or delays.  
The logic is quite simple:
* loads the cutscene sublevel
* when the sublevel is ready it plays the cutscene
* when the cutscene ends it unloads the sublevel and process the latent action to proceed with the execution flow

This system reads from the game instance the SFX sound class in order to mute all the SFXs and the *SkipCutsceneWidgetClass* to create the skip widget.  

There is also a *PreLoadCutscene* in order to preload the cutscene sublevel and the level sequence.  

Cutscenes can also be skipped.  
When the cutscene starts playing the player can hold a button to skip it.  
The *CutsceneSubsystem* creates a widget (*SkipCutsceneWidget*[.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/UI/SkipCutsceneWidget.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/UI/SkipCutsceneWidget.cpp)) and binds to the skip event which is broadcasted when the player holds the button.  
The skip logic broadcasts the *OnSkipRequested* event and stops the cutscene.  
The target level sequence (which registers itself at the *OnSkipRequested* event) then self-manages all the logic made during the cutscenes.  

**[⬆ Back to Top](#What-I-did)**

<a name="Level-loading"></a>
## 6. Level loading
The level loading system comprehends a logic for the asynchronous loading of a scene and one logic for the loading screen.  

### 6.1 SceneHandlerGISubsystem
The core logic is in this script: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/GISubsystem/SceneHandlerGISubsystem.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/GISubsystem/SceneHandlerGISubsystem.cpp).  
It derives from `UGameInstanceSubsystem` so that it is easily accessible from other scripts.  
We have two main functions that can be called to load a level:
* **LoadMapAsync**
* **SceneLoadingTransition**

#### 6.1.1 LoadMapAsync
It receives as input a pointer to the target `UWorld` and starts the loading; when the map is ready it broadcasts the *OnMapLoadComplete* delegate which can be used to open the loaded map by calling the *OpenLoadedMap* method.  

#### 6.1.2 SceneLoadingTransition
This method is the one used to load a level using a loading screen to cover the load.  
Three input parameters:
* pointer to the target `UWorld`
* class of the loading screen widget
* the minimum duration of the loading (which means the minimum screen time of the loading widget)

It creates a loading screen widget and calls its *StartLoading* function which will start the chain of logic that will lead to loading the target scene.

#### 6.1.3 Loading screen widget
The loading screen widget is based on the *LoadingTransition* script ([.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Public/UI/LoadingTransition.h)/[.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/blob/main/Source/TBO_01/Private/UI/LoadingTransition.cpp)).  
This script contains the methods to start/end the loading animations and interface with the *SceneHandlerGISubsystemé*.

#### 6.1.4 Logic chain
The logic chain that will lead to loading a target scene is as follow:
1. The *SceneLoadingTransition* method in the *SceneHandlerGISubsystem* is called; the loading widget is created and its *StartLoading* function is called;
2. The *StartLoadingAnimation* is played and when it’s over the *HandleStartLoadingOver* is called, subscribing to the *OnMapLoadComplete* delegate and calling the *LoadCachedMapAsync* method of the *SceneHandlerGISubsystem*.
3. The *LoadMapAsync* is called, loading the level (without opening it) and broadcasting the *OnMapLoadComplete* delegate when ready.
4. The map is then opened by a request of the widget if the *MinDuration* of the loading has elapsed, otherwise it waits the remaining seconds and then calls the opening with the *OpenLoadedMap* method.
5. When the map is opened the *EndLoadingAnimation* is played and at the end the widget is removed.

#### 6.1.5 Some notes
This loading transition logic it's not perfect, the problem is that when the level is loading you can see a freeze in the animations of the *LoadingTransition* widget because they run on the same thread of the game.  
To fix this I should have used slate, but I didn't had enough time to redo it.

**[⬆ Back to Top](#What-I-did)**
