#include "../Engine/Quaternion.h"
#include "Message.h"
#include <type_traits>


namespace messages {

	enum MessageType {
		MT_PLAYER_POSITION,
		MT_DYNAMIC_POSITION,
		MT_OTHER_PLAYER_POSITION,
		MT_LOAD_LEVEL_CUBE,
		MT_LOAD_LEVEL_PLAYER,
		MT_LOAD_LEVEL_OTHER,
		MT_KEY_PRESS
	};

	//PlayerPosition
	struct PayloadPlayerPosition {
		Vec3f pos;
	};

	struct PayloadDynamicPosition {
		int id;
		Vec3f pos;
		Quatf rot;
	};

	struct PayloadOtherPlayerPosition {
		Vec3f pos;
		int id;
	};

	//Load Cube
	struct PayloadLoadLevelCube {
		int cubeId;
		Vec3f pos;
		Vec3f scale;
		Quatf rot;
		bool dynamic;
		bool visible;
	};

	//Load Player
	struct PayloadLoadLevelPlayer {
		Vec3f pos;
	};


	//Load Other Player
	struct PayloadLoadLevelOther {
		int playerId;
		Vec3f pos;
	};

	struct PayloadKeyPress {
		char input;
		float rot;
	};


	template <typename T>
	MessageType getMessageType() {
		if constexpr (std::is_same<T, PayloadPlayerPosition>::value) {
			return MT_PLAYER_POSITION;
		}
		else if constexpr (std::is_same<T, PayloadDynamicPosition>::value) {
			return MT_DYNAMIC_POSITION;
		}
		else if constexpr (std::is_same<T, PayloadOtherPlayerPosition>::value) {
			return MT_OTHER_PLAYER_POSITION;
		}
		else if constexpr (std::is_same<T, PayloadLoadLevelCube>::value) {
			return MT_LOAD_LEVEL_CUBE;
		}
		else if constexpr (std::is_same<T, PayloadLoadLevelPlayer>::value) {
			return MT_LOAD_LEVEL_PLAYER;
		}
		else if constexpr (std::is_same<T, PayloadLoadLevelOther>::value) {
			return MT_LOAD_LEVEL_OTHER;
		}
		else if constexpr (std::is_same<T, PayloadKeyPress>::value) {
			return MT_KEY_PRESS;
		}
		else {
			static_assert(false,"Add message type above!");
		}
	}

	template <typename T>
	domnet::Message<T> m(getMessageType<T>());
	
	template <typename T>
	domnet::Message<T>*p;


	template <typename T>
	domnet::Message<T> *messagePnt() { return p<T>; };
	
	template <typename T>
	domnet::Message<T> &messageRef() { return m<T>; };

	
}
