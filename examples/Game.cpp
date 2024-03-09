#include "Systems.hpp"
#include "fef.hpp"

int main() {
	auto world = new fef::EcsWorld();

	for (size_t i = 0; i < 1000000; i++)
	{
		world->GetPool<uint32_t>()->Add(world->NewEntity());
	}

	auto systems = new fef::EcsSystems(world);

	systems
		->Add(new PlayerInitSystem(world))
		//->Add(new RunSystem(world))
		->Add(new InputSystem(world))
		->Add(new InputProcessSystem(world))
		->Add(new HelloCommandSystem(world))
		->Add(new CommandUnprocessedSystem(world));

	systems->Init();

	while (true) {
		systems->Run();
	}

	delete world;
	return 0;
}

int main3() {
	auto world = new fef::EcsWorld();
	
	auto CommandEventPool = world->GetPool<CommandEvent>();
	world->GetPool<CommandEvent>()->Add(world->NewEntity());
	auto query = world->Query<fef::With<CommandEvent>>();
	for (auto entity : *query) {
		
	}
	for (auto entity : *query) {
		
	}
	CommandEventPool->Del(0);
	CommandEventPool->Add(world->NewEntity());
	for (auto entity : *query) {

	}

	return 0;
}