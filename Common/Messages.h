#include "../Engine/Quaternion.h"
#include "Message.h"
#include <type_traits>


namespace Messages {

	enum MessageType {
		MT_PLAYER_POSITION,
		MT_LOAD_LEVEL_CUBE,
		MT_LOAD_LEVEL_PLAYER,
		MT_LOAD_LEVEL_OTHER,
	};

	//PlayerPosition
	struct PayloadPlayerPosition {
		int id;
		Vec3f pos;
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



	template <typename T>
	MessageType getMessageType() {
		if constexpr (std::is_same<T, PayloadPlayerPosition>::value) {
			return MT_PLAYER_POSITION;
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
