#include "fef.hpp"
#include "components.hpp"

struct PlayerInitSystem : public fef::EcsInitSystem {
	fef::EcsWorld* world;

	PlayerInitSystem(fef::EcsWorld* world) : world(world) { }

	void Init() {
		auto entity = world->NewEntity();
		auto unitPool = world->GetPool<Unit>();

		unitPool->Add(entity);
	}
};

struct RunSystem : public fef::EcsRunSystem {
	fef::EcsQuery* Query;

	RunSystem(fef::EcsWorld* world) {
		Query = world->Query<fef::With<User>>();
	}

	void Run() {
		for (auto entity : *Query) {
			std::cout << entity << std::endl;
		}
	}
};

struct InputSystem : public fef::EcsRunSystem {
	std::string Command;
	fef::EcsPool<CommandEvent>* CommandEventPool;

	InputSystem(fef::EcsWorld* world) {
		CommandEventPool = world->GetPool<CommandEvent>();
	}

	void Run() {
		Command.clear();

		auto& cmd = CommandEventPool->Add(CommandEventPool->World->NewEntity());
		std::cin >> cmd.Command;
	}
};

struct InputProcessSystem : public fef::EcsRunSystem {
	fef::EcsQuery* Commands;
	fef::EcsPool<CommandEvent>* CommandEventPool;
	fef::EcsPool<HelloCommand>* HelloCommandPool;

	InputProcessSystem(fef::EcsWorld* world) {
		CommandEventPool = world->GetPool<CommandEvent>();
		HelloCommandPool = world->GetPool<HelloCommand>();
		Commands = world->Query<fef::With<CommandEvent>>();
	}

	void Run() {
		for (auto entity : *Commands) {
			auto& cmd = CommandEventPool->Get(entity);
			if (cmd.Command == "Hello") {
				HelloCommandPool->Add(HelloCommandPool->World->NewEntity());
			}
			CommandEventPool->Del(entity);
		}
	}
};

struct HelloCommandSystem : public fef::EcsRunSystem {
	fef::EcsQuery *HelloCmds;
	fef::EcsPool<HelloCommand> *HelloCommandPool;

	HelloCommandSystem(fef::EcsWorld* world) {
		HelloCmds = world->Query<fef::With<HelloCommand>>();
		HelloCommandPool = world->GetPool<HelloCommand>();
	}

	void Run() {
		for (auto entity : *HelloCmds) {
			auto& cmd = HelloCommandPool->Get(entity);
			std::cout << "Hello!" << std::endl;
			HelloCommandPool->Del(entity);
		}
	}
};