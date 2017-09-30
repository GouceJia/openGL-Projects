#include "engine.hpp"
#include "draw.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
//#include <random>

#define PI 3.1415926535897932384626433832795
#define left 1
#define right 2
#define low 3
#define high 4
#define front 5
#define back 6

#define front_left 7
#define front_right 8
#define back_left 9
#define back_right 10

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class Car {
public:
    vec3 position;
    vec3 velocity;
    float collisionRadius;
    float thrustFactor, dragFactor;
    float angle, speed, turnRate;
    
    void draw() {
        Draw::drawCar(position);
    }

    float get_x() {
        return position.x;
    }
    float get_y() {
        return position.y;
    }
    float get_z() {
        return position.z;
    }
    float get_radius() {
        return collisionRadius;
    }
};

class Soccer {
public:
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
    float collisionRadius;

    void draw() {
        glColor3f(1.0, 1.0, 1.0);
        Draw::drawSoccer(position);
    }

    float get_x() {
        return position.x;
    }
    float get_y() {
        return position.y;
    }
    float get_z() {
        return position.z;
    }
    float get_radius() {
        return collisionRadius;
    }
};

class CarSoccer: public Engine {
public:
    SDL_Window *window;
    Car car;
    Soccer soccer;
    bool goal;
    bool speed_up;
    int carBall_collide;
    int ballWall_collide;
    int carWall_collide;

    float leftBound = -40;
    float rightBound = 40;
    float frontBound = 50;
    float backBound = -50;
    float highBound = 35;
    float lowBound = 0;
    float goalBound_x = 10;
    float goalBound_y = 10;

    vec3 leftNorm = vec3(1, 0, 0);
    vec3 rightNorm = vec3(-1, 0, 0);
    vec3 lowNorm = vec3(0, 1, 0);
    vec3 highNorm = vec3(0, -1, 0);
    vec3 frontNorm = vec3(0, 0, -1);
    vec3 backNorm = vec3(0, 0, 1);

    double generate_rand(){
        float init_random = (float) rand() / RAND_MAX * PI;
        srand(time(NULL));
        // std::random_device rd;
        // std::mt19937 rng(rd());
        // std::uniform_int_distribution<float> uni(0, PI);
        // auto random_float = uni(rng);
        // return (double) random_float;

    }


    CarSoccer() {
        window = createWindow("Car Soccer", 1280, 720);
        float angle_offset = 90;
        car.collisionRadius = 2.5;
        car.position = vec3(0, 1, 45); // center of car is 1 m above ground
        car.velocity = vec3(0, 0, 0);
        car.thrustFactor = 150;
        car.dragFactor = 5;
        car.angle = -90;
        car.speed = 0;
        car.turnRate = 2;

//        srand(time(NULL));
        double init_random = generate_rand();
        soccer.collisionRadius = 1;
        soccer.position = vec3(0, 8, 0);
        soccer.velocity = vec3(25*cos(init_random), 10, 25*sin(init_random));
        soccer.acceleration = vec3(0, -9.8, 0);  // modified gravity to be heavier

        carBall_collide = 0;
        ballWall_collide = 0;
        carWall_collide = 0;

        goal = false;
        speed_up = false;
    }

    ~CarSoccer() {
        SDL_DestroyWindow(window);
    }

    void run() {
        float fps = 60, timeStep = 1/fps;
        while (!shouldQuit()) {
            Engine::handleInput();
            simulate(timeStep);
            drawGraphics();
            Engine::waitForNextFrame(timeStep);
        }
    }

    /*  check if collision happens */
    void check_collision() {
        // ballWall_collide (also correct the position if reached out of feild)
        if ((soccer.get_x()-soccer.get_radius()) < leftBound) {
            ballWall_collide = left;
            soccer.position.x = leftBound + soccer.get_radius();
        }else if ((soccer.get_x()+soccer.get_radius()) > rightBound) {
            ballWall_collide = right;
            soccer.position.x = rightBound - soccer.get_radius();
        }else if ((soccer.get_y()-soccer.get_radius()) < lowBound) {
            ballWall_collide = low;
            soccer.position.y = lowBound + soccer.get_radius();
            //soccer.position.y = 0;
        }else if ((soccer.get_y()+soccer.get_radius() > highBound)) {
            ballWall_collide = high;
            soccer.position.y = highBound - soccer.get_radius();
        }else if ((soccer.get_z()+soccer.get_radius()) > frontBound) {
            ballWall_collide = front;
            soccer.position.z = frontBound - soccer.get_radius();
        }else if ((soccer.get_z()-soccer.get_radius()) < backBound) {
            ballWall_collide = back;
            soccer.position.z = backBound + soccer.get_radius();
        }

        // carWall_collide
          // detect special corner cases first
        if ((car.get_x() - car.get_radius()) < leftBound && 
			(car.get_z()+car.get_radius()) > frontBound) {
				carWall_collide = front_left;
				car.position.x = leftBound + car.get_radius();
				car.position.z = frontBound - car.get_radius();
		}else if ((car.get_x()+car.get_radius()) > rightBound &&
				  (car.get_z()+car.get_radius()) > frontBound) {
				carWall_collide = front_right;
				car.position.x = rightBound - car.get_radius();
				car.position.z = frontBound - car.get_radius();
		}else if ((car.get_x() - car.get_radius()) < leftBound && 
				  (car.get_z()-car.get_radius()) < backBound) {
				carWall_collide = back_left;
				car.position.x = leftBound + car.get_radius();
				car.position.z = backBound + car.get_radius();
		}else if ((car.get_x()+car.get_radius()) > rightBound &&
				  (car.get_z()-car.get_radius()) < backBound) {
				carWall_collide = back_right;
				car.position.x = rightBound - car.get_radius();
				car.position.z = backBound + car.get_radius();
		}else if ((car.get_x()-car.get_radius()) < leftBound) {
            carWall_collide = left;
            car.position.x = leftBound + car.get_radius();
        }else if ((car.get_x()+car.get_radius()) > rightBound) {
            carWall_collide = right;
            car.position.x = rightBound - car.get_radius();
        }else if ((car.get_y()-car.get_radius()) < lowBound) {
            carWall_collide = low;
            car.position.y = lowBound + car.get_radius();
        }else if ((car.get_y()+car.get_radius() > highBound)) {
            carWall_collide = high;
            car.position.y = highBound - car.get_radius();
        }else if ((car.get_z()+car.get_radius()) > frontBound) {
            carWall_collide = front;
            car.position.z = frontBound - car.get_radius();
        }else if ((car.get_z()-car.get_radius()) < backBound) {
            carWall_collide = back;
            car.position.z = backBound + car.get_radius();
        }
        
        

        // carBall_collide
//        float x2 = car.get_x() - soccer.get_x();
//        float y2 = car.get_y() - soccer.get_y();
//        float z2 = car.get_z() - soccer.get_z();
//        float distance = sqrt(x2*x2 + y2*y2 + z2*z2);
        float distance = cal_distance(car.position, soccer.position);
        if (distance <= (car.get_radius() + soccer.get_radius())) {
            carBall_collide = 1;
        }
    }

    /* check if goal (reset) */
    void check_goal() {
        if (soccer.position.z + soccer.collisionRadius >= frontBound ||
            soccer.position.z - soccer.collisionRadius <= backBound) {

            if (soccer.position.x - soccer.collisionRadius <= goalBound_x &&
                soccer.position.x + soccer.collisionRadius >= - goalBound_x) {

                if (soccer.position.y + soccer.collisionRadius <= goalBound_y &&
                    soccer.position.y - soccer.collisionRadius >= lowBound){
                    goal = true;
                    //SDL_DestroyWindow(window);
                }
            }
        }
    }

    float cal_distance(vec3 p1, vec3 p2) {
        float dis_x = p1.x - p2.x;
        float dis_y = p1.y - p2.y;
        float dis_z = p1.z - p2.z;

        float ret = sqrt(dis_x*dis_x + dis_y*dis_y + dis_z*dis_z);
        return ret;
    }

    void simulate(float timeStep) {
        // reset the game if goal
        check_goal();
        if (goal == true) {
            car.position = vec3(0, 1, 45);
            car.velocity = vec3(0, 0, 0);
            car.angle = -90;

            float init_random = generate_rand();
            soccer.position = vec3(0, 8, 0);
            soccer.velocity = vec3(25*cos(init_random), 10, 25*sin(init_random));

            goal = false;

        }

        // An oversimplified dynamics model for the car
        //getControlDirection();
        //vec2 dir = get_car_velocity();
        vec2 dir = getControlDirection();
        //if (glm::length(dir) > 0)
            //dir = glm::normalize(dir);
        if (speed_up == true) {
            car.dragFactor = 2.8;
        }
        //vec3 thrust = car.thrustFactor*vec3(dir.x, 0, -dir.y);
        //vec3 drag = car.dragFactor*car.velocity;
        //car.velocity += (thrust - drag)*timeStep;
        
        car.angle += dir.x * car.turnRate; //* car.speed;
        float thrust = car.thrustFactor * dir.y * 50;
        float drag = car.dragFactor * car.speed;
        car.speed += (thrust - drag) * timeStep;
        
        float xx = car.speed * cos(car.angle*PI/180);
		float yy = car.speed * sin(car.angle*PI/180);
        car.velocity = vec3(xx, 0, yy) * timeStep;
       
        
        car.position += car.velocity*timeStep;

        soccer.velocity += soccer.acceleration * timeStep;
        soccer.position += soccer.velocity * timeStep;

        // Handle ball/wall, car/wall, and ball/car collisions here
        check_collision();
        vec3 temp;
        float var;

        /* car-ball */
        if (carBall_collide != 0){
            float temp_x = soccer.get_x() - car.get_x();
            float temp_y = soccer.get_y() - car.get_y();
            float temp_z = soccer.get_z() - car.get_z();
            vec3 collisionNormal = vec3(temp_x, temp_y, temp_z);

            float center_distance = soccer.get_radius() + car.get_radius();

            // move the soccer away from car
            do {
                float xx = soccer.get_x() + collisionNormal.x * 1;
                float yy = soccer.get_y() + collisionNormal.x * 1;
                float zz = soccer.get_z() + collisionNormal.x * 1;
                soccer.position = vec3(xx, yy, zz);
            } while (cal_distance(soccer.position, car.position) <= center_distance);

            // compute and assign the new soccer-velocity
            vec3 tempSoc = soccer.velocity;
            vec3 tempCar = car.velocity;
            vec3 relative_velocity = vec3(tempSoc.x - tempCar.x, tempSoc.y - tempCar.y, tempSoc.z - tempCar.z);

//            vec3 flipped = vec3(4*collisionNormal.x, 4*collisionNormal.y, 4*collisionNormal.z)+relative_velocity;
//            temp = flipped + car.velocity;
//            soccer.velocity = vec3(4*temp_x, 4*temp_y, 4*temp.z);

            float var = -2*dot(relative_velocity, collisionNormal);
            vec3 flipped = vec3(var*collisionNormal.x, var*collisionNormal.y, var*collisionNormal.z)
                           + relative_velocity;
            temp = flipped + car.velocity;
            if (temp.y < 0 && soccer.get_x() < soccer.get_radius()){
                temp_y = 0;
            }
            soccer.velocity = vec3(0.15 * temp.x, 0.1 * temp.y, 0.15 * temp.z);

            carBall_collide = 0;
        }

        /* ball-wall */
        while (ballWall_collide != 0){
            switch (ballWall_collide) {
                case left:
                    //soccer.velocity = vec3(-2*leftNorm.x, -2*leftNorm.y, -2*leftNorm.z) + soccer.velocity;
                    //temp = vec3(10*leftNorm.x, 10*leftNorm.y, 10*leftNorm.z) + soccer.velocity;
                    var = -2*dot(leftNorm, soccer.velocity);
                    temp = vec3(var*leftNorm.x, var*leftNorm.y, var*leftNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
                    break;

                case right:
                    var = -2*dot(rightNorm, soccer.velocity);
                    temp = vec3(var*rightNorm.x, var*rightNorm.y, var*rightNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
                    break;

                case low:
                    var = -2*dot(lowNorm, soccer.velocity);
                    if (var < 0) {
                        var = 0;
                    }
                    temp = vec3(var*lowNorm.x, var*lowNorm.y, var*lowNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
//                    if (soccer.position.y - soccer.get_radius() < lowBound) {
//                        soccer.position.y = 0;
//                    }
                    break;

                case high:
                    var = -2*dot(highNorm, soccer.velocity);
                    temp = vec3(var*highNorm.x, var*highNorm.y, var*highNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
                    break;

                case front:
                    var = -2*dot(frontNorm, soccer.velocity);
                    temp = vec3(var*frontNorm.x, var*frontNorm.y, var*frontNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
                    break;

                case back:
                    var = -2*dot(backNorm, soccer.velocity);
                    temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + soccer.velocity;
                    soccer.velocity = temp * 0.8f;
                    ballWall_collide = 0;
                    break;
            }
        }

        /* car-wall */
        while (carWall_collide != 0){
            switch (carWall_collide) {
                case left:
                    //temp = vec3(3*leftNorm.x, 3*leftNorm.y, 3*leftNorm.z) + car.velocity;
                    //car.velocity = temp * 0.8f;
                    //carWall_collide = 0;
                    var = -2*dot(leftNorm, car.velocity);
                    temp = vec3(var*leftNorm.x, var*leftNorm.y, var*leftNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;

                case right:
                    var = -2*dot(rightNorm, car.velocity);
                    temp = vec3(var*rightNorm.x, var*rightNorm.y, var*rightNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;

                case low:
                    var = -2*dot(lowNorm, car.velocity);
                    temp = vec3(var*lowNorm.x, var*lowNorm.y, var*lowNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;

                case high:
                    var = -2*dot(highNorm, car.velocity);
                    temp = vec3(var*highNorm.x, var*highNorm.y, var*highNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;

                case front:
                    var = -2*dot(frontNorm, car.velocity);
                    temp = vec3(var*frontNorm.x, var*frontNorm.y, var*frontNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;

                case back:
                    var = -2*dot(backNorm, car.velocity);
                    temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;
                    
                case front_left:
					var = -2*dot(frontNorm, car.velocity);
                    temp = vec3(var*frontNorm.x, var*frontNorm.y, var*frontNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;
                
                case front_right:
					var = -2*dot(frontNorm, car.velocity);
                    temp = vec3(var*frontNorm.x, var*frontNorm.y, var*frontNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;
                    
                case back_left:
					var = -2*dot(backNorm, car.velocity);
                    temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;
                
                case back_right:
					var = -2*dot(backNorm, car.velocity);
                    temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + car.velocity;
                    car.velocity = temp * 0.8f;
                    carWall_collide = 0;
                    break;
            }
        }
        
        //while (carWall_collide != 0) {
			//if (carWall_collide == left) {
				//var = -2*dot(leftNorm, car.velocity);
				//temp = vec3(var*leftNorm.x, var*leftNorm.y, var*leftNorm.z) + car.velocity;
				//car.velocity = temp * 0.8f;
			//} else if (carWall_collide == right){
				//var = -2*dot(rightNorm, car.velocity);
				//temp = vec3(var*rightNorm.x, var*rightNorm.y, var*rightNorm.z) + car.velocity;
				//car.velocity = temp * 0.8f;
			//} else if (carWall_collide == low) {
				//var = -2*dot(lowNorm, car.velocity);
                //temp = vec3(var*lowNorm.x, var*lowNorm.y, var*lowNorm.z) + car.velocity;
                //car.velocity = temp * 0.8f;
			//} else if (carWall_collide == high) {
				//var = -2*dot(highNorm, car.velocity);
				//temp = vec3(var*highNorm.x, var*highNorm.y, var*highNorm.z) + car.velocity;
				//car.velocity = temp * 0.8f;
			//} else if (carWall_collide == front) {
				//var = -2*dot(backNorm, car.velocity);
				//temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + car.velocity;
				//car.velocity = temp * 0.8f;
			//} else if (carWall_collide == back) {
				//var = -2*dot(backNorm, car.velocity);
				//temp = vec3(var*backNorm.x, var*backNorm.y, var*backNorm.z) + car.velocity;
				//car.velocity = temp * 0.8f;
			//}
		
			//carWall_collide = 0;
		
		//}

    }

    vec2 getControlDirection() {
        int speedVar = 1;
        vec2 dir(0,0);
        if (Engine::isKeyDown(SDL_SCANCODE_LEFT))
            dir.x -= speedVar;
        if (Engine::isKeyDown(SDL_SCANCODE_RIGHT))
            dir.x += speedVar;
        if (Engine::isKeyDown(SDL_SCANCODE_UP))
            dir.y += speedVar;
        if (Engine::isKeyDown(SDL_SCANCODE_DOWN))
            dir.y -= speedVar;
        return dir;
    }
    
    //void getControlDirection() {
		//float speed_var = 1;
		//float rotate_var = 2;
        //if (Engine::isKeyDown(SDL_SCANCODE_LEFT))
            //car.angle += rotate_var;
        //if (Engine::isKeyDown(SDL_SCANCODE_RIGHT))
            //car.angle -= rotate_var;
        //if (Engine::isKeyDown(SDL_SCANCODE_UP))
            //car.speed += speed_var;
        //if (Engine::isKeyDown(SDL_SCANCODE_DOWN))
            //car.speed -= speed_var;
		//}
	
	//vec2 get_car_velocity() {
		////float temp_x = car.velocity.x;
		////float temp_y = car.velocity.z;
		//float xx = car.speed * cos(car.angle);
		//float yy = car.speed * sin(car.angle);
		//vec2 dir(0, 0);
		//dir.x = xx;
		//dir.y = yy;
		//}

    void addLight(GLenum light, vec4 position, vec3 color) {
        glEnable(light);
        glLightfv(light, GL_POSITION, &position[0]);
        glLightfv(light, GL_DIFFUSE, &color[0]);
    }
    
    void drawCar_rotate() {
		glPushMatrix();
        glTranslatef(car.position.x, car.position.y, car.position.z);
        glRotatef(90-car.angle, 0.0f, 1.0f, 0.0f);
        glTranslatef(-car.position.x, -car.position.y, -car.position.z);
        car.draw();
        glPopMatrix();
		}

    void drawGraphics() {
        // Allow lines to show up on top of filled polygons
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);
        // Set up the camera in a good position to see the entire field
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 16/9., 1,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,60,70, 0,0,10, 0,1,0);
        // Clear window
        glClearColor(0.2,0.2,0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Add some lights
        addLight(GL_LIGHT0, vec4(+1,0.5,+1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT1, vec4(+1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT2, vec4(-1,0.5,-1,0), vec3(0.5,0.5,0.5));
        addLight(GL_LIGHT3, vec4(-1,0.5,+1,0), vec3(0.5,0.5,0.5));
        glEnable(GL_LIGHTING);
        // Draw the ground as a quadrilateral
        glColor3f(0.3,0.6,0.2);
        glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(-50,0,-60);
        glVertex3f(+50,0,-60);
        glVertex3f(+50,0,+60);
        glVertex3f(-50,0,+60);
        glEnd();
        // Draw the car
        //car.draw();
        //car.angle += 90;
        drawCar_rotate();
        //car.angle -= 90;
        
        glDisable(GL_LIGHTING);

        // Draw the field borders, the pitch markings, and the goals here
        Draw::drawField();

        glEnable(GL_LIGHTING);


        // Draw the ball
        soccer.draw();
        Draw::drawShadow(soccer.position);

        SDL_GL_SwapWindow(window);
    }

    void onKeyDown(SDL_KeyboardEvent &e) {
        // Check whether the space bar was pressed, and if so, reset the ball
        // to the center of the pitch and give it a "kick-off" velocity.
        // We found that a nice initial velocity is (25 cos(a), 10, 25 sin(a))
        // where a is a random number between 0 and pi.
        double init_random;

        switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.keysym.sym) {
                    case SDLK_SPACE:
                        soccer.position = vec3(0, 8, 0);
                        init_random = generate_rand();
                        soccer.velocity = vec3(25*sin(init_random), 10, -25*cos(init_random));
                        break;

                    case SDLK_LSHIFT:
                        //car.velocity = vec3(2*car.velocity.x, 2*car.velocity.y, 2*car.velocity.z);
                        speed_up = true;
                        break;

                    case SDLK_RSHIFT:
                        //car.velocity = vec3(2*car.velocity.x, 2*car.velocity.y, 2*car.velocity.z);
                        speed_up = true;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (e.keysym.sym) {
                    case SDLK_LSHIFT:
                        speed_up = false;
                        break;

                    case SDLK_RSHIFT:
                        speed_up = false;
                        break;
                }
                break;
        }

    }

};

int main(int argc, const char *argv[]) {
    CarSoccer game;
    game.run();
}
