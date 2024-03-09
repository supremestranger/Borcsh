/*void some_func(int& param) {
	param += 3;
}

class Parent {
	public: virtual ~Parent() {}
};

class Child {

};

int main4() {
	auto world = new EcsWorld();
	auto unitPool = world->GetPool<unit>();
	auto ent1 = world->NewEntity();
	auto &unit = unitPool->Add(ent1);
	unit = unitPool->GetMut(ent1);

	std::cout << unit.health << std::endl;
	unitPool->Del(ent1);

	auto ent2 = world->NewEntity();
	auto &unit2 = unitPool->Add(ent2);
	unit.health = 27;
	std::cout << unit2.health << std::endl;

	return 0;
}


}*/