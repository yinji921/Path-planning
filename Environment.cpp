#include "Environment.hpp"

// VectorPoint

double VectorPoint::length() const {
	return sqrt(x * x + y * y);
}

void VectorPoint::info() {
	std::cerr << "[Debug] x = " << x << ", y = " << y << std::endl;
}

VectorPoint::VectorPoint() {
    x = 0;
    y = 0;
}

VectorPoint::VectorPoint(double initX, double initY) {
	x = initX;
	y = initY;
}

VectorPoint VectorPoint::operator-(const VectorPoint &c) const {
	return VectorPoint(x - c.x, y - c.y);
}

VectorPoint VectorPoint::operator+(const VectorPoint &c) const {
	return VectorPoint(x + c.x, y + c.y);
}

double VectorPoint::operator*(const VectorPoint &c) const {
	return (x * c.x + y * c.y);
}

VectorPoint VectorPoint::operator*(int scalar) const {
	return VectorPoint(x * scalar, y * scalar);
}

VectorPoint VectorPoint::operator/(int len) const {
	return VectorPoint(x / len, y / len);
}

double VectorPoint::cosin(const VectorPoint &c) {
	return (VectorPoint(x, y) * c) / this->length() / c.length();
}

VectorPoint VectorPoint::calAngle(double angle, double dist) const {
	return VectorPoint(dist * sin(angle * PI / 180.0), dist * cos(angle * PI / 180.0));
}


// Obstacle

Obstacle::Obstacle() {
	x = y = kind = r = a = b = 0;
}

Obstacle::Obstacle(int X, int Y, int Kind, int R, int A, int B) {
	x = X;
	y = Y;
	kind = Kind;
	r = R;
	a = A;
	b = B;
}

void Obstacle::set(int X, int Y, int Kind, int R, int A, int B) {
	x = X;
	y = Y;
	kind = Kind;
	r = R;
	a = A;
	b = B;
}

bool Obstacle::isIn(int testX, int testY) {
	switch (kind) {
		case 1: {
			if ((testX - x) * (testX - x) + (testY - y) * (testY - y) <= r * r)
				return true;
			return false;
		}
		case 0: {
			if ((abs(testX - x) < a / 2.0) && (abs(testY - y) < b / 2.0)) 
				return true;
			return false;
		}
	}
}

bool Obstacle::isIn(VectorPoint &v) {
    int testX = v.x;
    int testY = v.y;
	switch (kind) {
		case 1: {
			if ((testX - x) * (testX - x) + (testY - y) * (testY - y) <= r * r)
				return true;
			return false;
		}
		case 0: {
			if ((abs(testX - x) < a / 2.0) && (abs(testY - y) < b / 2.0)) 
				return true;
			return false;
		}
	}
}

VectorPoint Obstacle::vectorForm() {
    return VectorPoint(x, y);
}



// Environment

Environment::Environment() {

	srand((unsigned int) time(NULL));
	for (int i = 0; i < numObstacles; i++) {
		switch (rand() % 2) {
			case 0: {
				Obstacle temp(rand() % height, rand() % width, 0, 0, rand() % (height / 5) + 10, rand() % (height / 5) + 10);
				obstacles.push_back(temp);
				break;
			}
			case 1: {
				Obstacle temp(rand() % height, rand() % width, 1, rand() % (height / 5) + 10, 0, 0);
				obstacles.push_back(temp);
				break;
			}
		}
	}
	
	Obstacle temp(20, 20, 0, 0, 20, 20);
	obstacles.push_back(temp);
	temp.set(height - 20, width - 20, 0, 0, 20, 20);
	obstacles.push_back(temp);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bool within = false;
			for (int k = 0; k < numObstacles; k++) {
				if (obstacles[k].isIn(i, j)) {
					within = true;
				}
			}
			
            if (within) {
                Map[i][j] = 2;
            } else {
                Map[i][j] = 3;
            }
            
            if (obstacles[numObstacles].isIn(i, j)) {
                Map[i][j] = 0;
            }
            
            if (obstacles[numObstacles + 1].isIn(i, j)) {
                Map[i][j] = 1;
            }            
		}
	}
}

Environment::Environment(int mapset) {
    switch(mapset) {
        case 1: // Normal
        	obstacles.push_back(Obstacle(100, 100, 0, 0, 200, 50));
        	obstacles.push_back(Obstacle(350, 200, 0, 0, 300, 50));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	break;
        case 2: // Simple
        	obstacles.push_back(Obstacle(100, 100, 0, 0, 100, 50));
        	obstacles.push_back(Obstacle(200, 200, 0, 0, 50, 100));
        	obstacles.push_back(Obstacle(300, 300, 0, 0, 100, 50));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	break;   
        case 3: // Narrow (Hard) 
        	obstacles.push_back(Obstacle(145, 0, 0, 0, 210, 920));
        	obstacles.push_back(Obstacle(375, 500, 0, 0, 170, 920));
        	obstacles.push_back(Obstacle(300, 300, 1, 0, 0, 0));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
        	break;      
        case 4: // Trap (Hard)
            obstacles.push_back(Obstacle(300, 200, 0, 0, 50, 250));
            obstacles.push_back(Obstacle(200, 300, 0, 0, 250, 50));
            obstacles.push_back(Obstacle(450, 175, 0, 0, 100, 400));
            obstacles.push_back(Obstacle(175, 450, 0, 0, 400, 100));
            obstacles.push_back(Obstacle(300, 200, 1, 0, 0, 0));
            break;
        case 5: // Simple
            obstacles.push_back(Obstacle(50, 325, 0, 0, 100, 400));
            obstacles.push_back(Obstacle(325, 50, 0, 0, 400, 100));
            obstacles.push_back(Obstacle(450, 175, 0, 0, 100, 400));
            obstacles.push_back(Obstacle(175, 450, 0, 0, 400, 100));
            obstacles.push_back(Obstacle(250, 250, 1, 50, 0, 0));
            break;
    }
	
	Obstacle temp(20, 20, 0, 0, 20, 20);
	obstacles.push_back(temp);
	temp.set(height - 20, width - 20, 0, 0, 20, 20);
	obstacles.push_back(temp);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bool within = false;
			for (int k = 0; k < numObstacles; k++) {
				if (obstacles[k].isIn(i, j)) {
					within = true;
				}
			}
			
            if (within) {
                Map[i][j] = 2;
            } else {
                Map[i][j] = 3;
            }
            
            if (obstacles[numObstacles].isIn(i, j)) {
                Map[i][j] = 0;
            }
            
            if (obstacles[numObstacles + 1].isIn(i, j)) {
                Map[i][j] = 1;
            }            
		}
	}
}

std::string Environment::mapToString() {
	std::string output;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			switch (this->at(i, j)) {
				case 0:
					output += 's';
					break;
				case 1:
					output += 't';				
					break;
				case 2:
					output += '#';
					break;
				case 3:
					output += '.';
					break;
			}
		}
		output += '\n';
	}
	return output;
}

void Environment::mapInfo() {
	std::cerr << "[Info] Map height: " << height << ", width = " << width << std::endl;
	for (int i = 0; i < numObstacles; i++) {
		std::cerr << "[Info] Obstacle " << i;
		switch (obstacles[i].kind) {
			case 0:
				std::cerr << " (rectangle): x = " << obstacles[i].x  << ", y = " << obstacles[i].y << ", a = " << obstacles[i].a << ", b = " << obstacles[i].b << std::endl;
				break;
			case 1:
				std::cerr << " (circle): x = " << obstacles[i].x  << ", y = " << obstacles[i].y << ", r = " << obstacles[i].r << std::endl;
				break;
		}
	}
}

int Environment::at(double x, double y) {
	return Map[(int) round(x)][(int) round(y)];
}

int Environment::at(int x, int y) {
	if (0 <= x && x < height && 0 <= y && y < height)
		return Map[x][y];
	else 
		return -1;
}

int Environment::at(VectorPoint c) {
	return this->at(c.x, c.y);
}


VectorPoint Environment::closestPoint(const VectorPoint &c, int index) {
	switch (obstacles[index].kind) {
		case 0: {
			if (c.x < obstacles[index].x - obstacles[index].a / 2) {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return VectorPoint(obstacles[index].x - obstacles[index].a / 2, obstacles[index].y - obstacles[index].b / 2);
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return VectorPoint(obstacles[index].x - obstacles[index].a / 2, obstacles[index].y + obstacles[index].b / 2);
				} else {
					return VectorPoint(obstacles[index].x - obstacles[index].a / 2, c.y);
				}
			} else 
			if (c.x > obstacles[index].x + obstacles[index].a / 2) {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return VectorPoint(obstacles[index].x + obstacles[index].a / 2, obstacles[index].y - obstacles[index].b / 2);
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return VectorPoint(obstacles[index].x + obstacles[index].a / 2, obstacles[index].y - obstacles[index].b / 2);
				} else {
					return VectorPoint(obstacles[index].x + obstacles[index].a / 2, c.y);
				}
			} else {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return VectorPoint(c.x, obstacles[index].y - obstacles[index].b / 2);
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return VectorPoint(c.x, obstacles[index].y + obstacles[index].b / 2);
				} else {
					return VectorPoint(0, 0);
				}
			}
			break;
		}
		case 1: {
			if ((c - VectorPoint(obstacles[index].x, obstacles[index].y)).length() <= obstacles[index].r)
				return VectorPoint(0, 0);
			else 
				return VectorPoint(obstacles[index].x, obstacles[index].y);
			break;
		}
	}
}

double Environment::calDist(const VectorPoint &c, int index) {
	switch (obstacles[index].kind) {
		case 0: {
			if (c.x < obstacles[index].x - obstacles[index].a / 2) {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return r(s(obstacles[index].x - obstacles[index].a / 2 - c.x) + s(obstacles[index].y - obstacles[index].b / 2 - c.y));
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return r(s(obstacles[index].x - obstacles[index].a / 2 - c.x) + s(obstacles[index].y + obstacles[index].b / 2 - c.y));
				} else {
					return r(s(obstacles[index].x - obstacles[index].a / 2 - c.x));
				}
			} else 
			if (c.x > obstacles[index].x + obstacles[index].a / 2) {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return r(s(obstacles[index].x + obstacles[index].a / 2 - c.x) + s(obstacles[index].y - obstacles[index].b / 2 - c.y));
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return r(s(obstacles[index].x + obstacles[index].a / 2 - c.x) + s(obstacles[index].y + obstacles[index].b / 2 - c.y));
				} else {
					return r(s(obstacles[index].x + obstacles[index].a / 2 - c.x));
				}
			} else {
				if (c.y < obstacles[index].y - obstacles[index].b / 2) {
					return r(s(obstacles[index].y - obstacles[index].b / 2 - c.y));
				} else 
				if (c.y > obstacles[index].y + obstacles[index].b / 2) {
					return r(s(c.y - obstacles[index].y - obstacles[index].b / 2));
				} else {
					return 0;
				}
			}
			break;
		}
		case 1: {
			if ((c - VectorPoint(obstacles[index].x, obstacles[index].y)).length() <= obstacles[index].r)
				return 0;
			else 
				return (c - VectorPoint(obstacles[index].x, obstacles[index].y)).length() - obstacles[index].r;
			break;
		}
	}
}

double Environment::potential(const VectorPoint& c) {
	double gamma = 2, k = 10, range = 5000, sum = 0;
	VectorPoint totalForce(0, 0);
	for (int i = 0; i < numObstacles; i++) {
		double distance = calDist(c, i) + 0.001;
		double p = range / gamma * s(1 / distance - 1 / range);
		
		if ((obstacles[i].kind == 1) && (obstacles[i].r == 0))
		    continue;
        sum += p;
	}
    
    double ka = 1.0;
	VectorPoint start = c - obstacles[numObstacles].vectorForm();
	sum += ka * start.length();
	
	//VectorPoint end = c - obstacles[numObstacles + 1].vectorForm();
	//sum -= ka * end.length();
	
	return sum;
}

std::vector<VectorPoint> Environment::nextPropagation(const VectorPoint& x, const VectorPoint& y, int dist) {
	std::vector<VectorPoint> result;
	VectorPoint dir = x - y;
	for (int i = 0; i < 360; i += 15) {
		VectorPoint angle = x.calAngle(i, dist);
		if ((angle.cosin(dir) >= 0.697106781) && (this->at(x + angle) != 2) && ((x + angle).x >= 0) && ((x + angle).y >= 0) && (int((x + angle).x) < height) && (int((x + angle).y) < width)) {
			result.push_back(x + angle);
		}
	}
	return result;
}


bool Environment::blocked(const VectorPoint& x, const VectorPoint& y) {
    
    VectorPoint dir = y - x;
    VectorPoint start = x;
    
    double totalLength = dir.length();
    dir = dir / dir.length();
    VectorPoint unit = dir;
    
    while (dir.length() < totalLength) {
        if (this->at(x + dir) == 2) {
            return false;
        }
        dir = dir + unit;
    }
    
    return true;
}

VectorPoint Environment::startPoint() {
    return VectorPoint(obstacles[numObstacles].x, obstacles[numObstacles].y);
}

VectorPoint Environment::endPoint() {
    return VectorPoint(obstacles[numObstacles + 1].x, obstacles[numObstacles + 1].y);
}


