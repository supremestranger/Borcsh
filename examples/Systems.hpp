#include "fef.hpp"
#include "components.hpp"
#include <functional>

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
	fef::EcsPool<Unprocessed> *UnprocessedPool;

	InputSystem(fef::EcsWorld* world) {
		CommandEventPool = world->GetPool<CommandEvent>();
		UnprocessedPool = world->GetPool<Unprocessed>();
	}

	void Run() {
		Command.clear();

		auto entity = CommandEventPool->World->NewEntity();
		auto& cmd = CommandEventPool->Add(entity);
		UnprocessedPool->Add(entity);
		
		std::cin >> cmd.Command;
	}
};

struct InputProcessSystem : public fef::EcsRunSystem {
	fef::EcsQuery* Commands;
	fef::EcsPool<CommandEvent>* CommandEventPool;
	fef::EcsPool<HelloCommand>* HelloCommandPool;

	std::unordered_map<std::string, std::function<void(fef::EcsWorld*, uint32_t)>> Callbacks;

	InputProcessSystem(fef::EcsWorld* world) {
		CommandEventPool = world->GetPool<CommandEvent>();
		HelloCommandPool = world->GetPool<HelloCommand>();
		Commands = world->Query<fef::With<CommandEvent>>();
		Callbacks.emplace("Hello", [](fef::EcsWorld* w, uint32_t entity) { w->GetPool<HelloCommand>()->Add(entity); });
	}

	void Run() {
		for (auto entity : *Commands) {
			auto& cmd = CommandEventPool->Get(entity);
			if (Callbacks.contains(cmd.Command)) {
				Callbacks.at(cmd.Command)(CommandEventPool->World, entity);
				CommandEventPool->Del(entity);
			}
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

struct CommandUnprocessedSystem : fef::EcsRunSystem {
	fef::EcsQuery *Commands;
	fef::EcsPool<CommandEvent> *CommandEventPool;

	CommandUnprocessedSystem(fef::EcsWorld *world) {
		Commands = world->Query<fef::With<CommandEvent, Unprocessed>>();
		CommandEventPool = world->GetPool<CommandEvent>();
	}

	void Run() {
		for (auto entity : *Commands) {
			auto &cmd = CommandEventPool->Get(entity);

			std::cout << "No command found: " << cmd.Command << std::endl;
			CommandEventPool->Del(entity);
			CommandEventPool->World->GetPool<Unprocessed>()->Del(entity); 
		}
	}
};