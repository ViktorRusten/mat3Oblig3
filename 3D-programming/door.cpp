#include "door.h"
#include "transform.h"
#include "renderwindow.h"

Door::Door()
{

}

Door::Door(RenderWindow *Owner, Vec3 Position, Quaternion Rotation, Vec3 colors)
{
    planeMesh = new Plane(Owner, Position, Rotation, colors);
    planeMesh->getTransform().setScale(Vec3(0.3f, 0.2f, 0.3f));
}

void Door::update(float deltaTime)
{
    if(open == true)
    {
        if(xAxis > -0.2f)
        {

            planeMesh->getTransform().setPosition(Vec3(-1.4f, 0.4f, 0.f));
        }
    }
}

void Door::createObject()
{
    planeMesh->createObject();
}

void Door::drawObject()
{
    planeMesh->drawObject();
}

bool Door::openDoor()
{
    open = !open;
}


