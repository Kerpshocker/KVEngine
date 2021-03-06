#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "Manager.h"
#include <hash_map>

namespace Input
{
	enum Keys
	{
		KEY_MOVE_FORWARD = 1,	//right pedal - pos
		KEY_MOVE_BACKWARD = 2,	//right pedal - neg
		KEY_PITCH_NEG = 4,		//right joystick forw
		KEY_PITCH_POS = 8,		//right joystick back
		KEY_YAW_NEG = 16,		//right joystick left
		KEY_YAW_POS = 32,		//right joystick right
		KEY_ROLL_NEG = 64,		//left joystick left
		KEY_ROLL_POS = 128,		//left joystick right
		KEY_Z_TRIGGER = 256		//trigger behind right joystick
	};

	class InputManager : public KVE::Utilities::Manager
	{
		SINGLETON_INSTANCE( InputManager );
	public:
		void initialize( void );
		void release( void );

		int	 getKeyData( void ) const;
		bool isKeyPressed( Keys key ) const;
		void registerKeyPress( int key );
		void registerKeyRelease( int key );

	private:
		int keyMap[ 26 ];
		int keyData;
	};

	// convenience functions
	inline void Initialize( void ) { InputManager::Instance().initialize(); }
	inline void Release( void ) { InputManager::Instance().release(); }
	inline int  GetKeyInputData( void ) { return InputManager::Instance().getKeyData(); }
	inline bool IsKeyPressed( Keys key ) { return InputManager::Instance().isKeyPressed( key ); }
	inline void RegisterKeyPress( int key ) { InputManager::Instance().registerKeyPress( key ); }
	inline void RegisterKeyRelease( int key ) { InputManager::Instance().registerKeyRelease( key ); }
}

#endif

