#include "Base3D.h"
#include <math.h>
#include "RenderHelpTools.h"
#include "Point.h"
#include "World.h"
#include "UniversalPoint.h"

using namespace base3D;
using namespace utils;


CoOrdSysManager::CoOrdSysManager() : systems(*new vector<CoOrdinateSystem>(1)){
	globalCoOrdinateSystem = &systems[0];
	clearSpots = vector<int>(0);
	systems[0].id = 0;
	systems[0].origin = Point(0, 0, 0);
}


childCoOrdSys::childCoOrdSys(CoOrdinateSystem* _parent, Point _origin, int _Id) : parent(_parent) {
	origin = _origin;
	id = _Id;
	//TODO: register with coOrdSysManager?
}






WorldObject::WorldObject(World* newWorld) {
	world = newWorld;
}

void WorldObject::setWorld(World* newWorld) {
	world = newWorld;
}



int CoOrdSysManager::newCoOrdSys(Point origin)
{
	int id;
	if (clearSpots.size() > 0) {
		id = clearSpots[0];
		systems[id] = static_cast<CoOrdinateSystem>(childCoOrdSys(globalCoOrdinateSystem, origin, id));
	} else {
		id = int(systems.size());
		systems.push_back(childCoOrdSys((globalCoOrdinateSystem), origin, id));
	}
	return id;
}

void CoOrdSysManager::removeCoOrdSys(int id)
{
	delete &systems[id];
	systems[id] = CoOrdinateSystem();
	clearSpots.push_back(id);
}

void CoOrdSysManager::setSystemOrigin(int id, Point p) {
	systems[id].origin = p;
}

CoOrdinateSystem CoOrdSysManager::getSystem(int id) {
	return systems[id];
}

CoOrdinateSystem & CoOrdinateSystem::operator=(CoOrdinateSystem newCoOrdinateSystem)
{
	CoOrdinateSystem c = CoOrdinateSystem(newCoOrdinateSystem);
	swap(id, c.id);
	swap(origin, c.origin);
	return *this;
}
