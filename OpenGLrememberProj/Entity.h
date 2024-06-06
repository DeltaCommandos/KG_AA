//#pragma once
//
//class Entity {
//
//};
//
//class Mob : public Entity {
//protected:
//	int damage;
//	float speed;
//	clock_t last, delay;
//public:
//	Sphere body;
//
//	Mob(int hp, int dmg, float spd, Vector3 pos, float phi, EntityContainer* hst, clock_t dlay = 500) : Entity(hp, hst) {
//		damage = dmg;
//		body.pos = pos + Vector3(0, 0, body.scale.Z());
//		body.angle = phi;
//		//body. RenderObject();
//		speed = spd;
//		delay - dlay;
//		last = clock();
//		host->mobs.push_back(*this);
//	}
//
//	void Shoot() {
//		clock_t now = clock();
//		if (now - last >= delay) {
//			last = now;
//			Projectile(damage, .6, body.pos + Vector3(cos(body.angle) * (body.scale.X() + epsilon), sin(body.angle) * (body.scale.Y() + epsilon));
//		}
//	}
//
//
//};
//
