#include "Environment.hpp"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <time.h>


using namespace std;
using namespace cv;


//struct of vertex
struct vertex{
  int nextnum = 0;
  vertex *next[100] = {NULL};
  vertex *last = NULL;
  VectorPoint self;
};
//return distance between two points
double dis(VectorPoint A, VectorPoint B){
  return sqrt(pow((A.x-B.x),2)+pow((A.y-B.y),2));
}
//generate a random point on the map
VectorPoint Random(Environment test){
  while(true){
  int x = rand()%499;
  int y = rand()%499;

  if(test.at(x,y) == 3 || test.at(x,y) == 1)// if the point is valuable
    return VectorPoint(x,y);
  }
}
void drawline(Mat image, VectorPoint A, VectorPoint B){
  line(image, Point(int(A.x), int(A.y)), Point(int(B.x), int(B.y)), Scalar(255, 0, 0));
  return;
}
void drawObstacle(Mat &image, Obstacle ob) {
    if (ob.kind == 1) {
        circle(image, Point(ob.x, ob.y), int(ob.r), Scalar(255, 0, 0));
    } else 
    if (ob.kind == 0) {
        rectangle(image, Point(ob.x - ob.a / 2, ob.y - ob.b / 2), Point(ob.x + ob.a / 2, ob.y + ob.b / 2), Scalar(255, 0, 0));
    }
}
int traceback(Environment &test, vertex &A){
  if(test.at(A.self.x, A.self.y) != 0){
    cout<<"line"<<endl;
    cout<<"255"<<endl;
    cout<<A.self.x<<" "<<A.self.y<<endl;
    cout<<A.last->self.x<<" "<<A.last->self.y<<endl;
    cout<<"endline"<<endl;
    return traceback(test, *A.last) + 1;
  }
  return 0;
}

int main() {
  clock_t TotalTime = clock();
  Mat image(500, 500, 6);
  vertex store[5000];//store vertex
  int vnum = 1; //record the number of vertex
  Environment test(3);

  
  cout<<"start"<<endl;
  cout<<test.obstacles[5].x<<" "<<test.obstacles[5].y<<endl;
  cout<<"end"<<endl;
  cout<<test.obstacles[6].x<<" "<<test.obstacles[6].y<<endl;

  
  for(int i = 0; i < 5; i++ ){
    cout<<"obstacle"<<endl;
    cout<<test.obstacles[i].x<<" "<<test.obstacles[i].y<<" "<<test.obstacles[i].kind<<" "<<test.obstacles[i].r<<" "<<test.obstacles[i].a<<" "<<test.obstacles[i].b<<endl;
  }
  cout<<"clear"<<endl;
  

  

  VectorPoint init(20,20);
  VectorPoint goal(480,480);
  store[0].self = init;


  while(true){
    double min = 100000;
    int index = 0;// indicate the nearest vertex 
    
    //take a random point
    VectorPoint rand = Random(test);

    //find the vertex closest to the random point
    for(int i=1;i<vnum;i++){
      if(min > dis(store[i].self,rand)){
	min = dis(store[i].self,rand);
	index = i;
      }
    }

    
    //extend this vertex for possible next moves
    vector<VectorPoint> result;
    if(vnum == 1){
      result = test.nextPropagation(store[index].self,goal, 2);
    }
    else{
      result = test.nextPropagation(store[index].self,store[index].last->self, 4);
    }
    double rmin = 100000;
    int rindex = 0;// indicates the most applicable vertex returned by propagation function

    if(result.size() != 0){
      for(int i = 0; i < result.size(); i++){//check all extentions
	if(test.at(result[i].x, result[i].y) == 1 || test.at(result[i].x, result[i].y) == 3){//check for obstacle
	  if(rmin > dis(result[i], rand)){
	    rmin = dis(result[i], rand);
	    rindex = i;
	  }
	}
      }
    }
    else continue;
  
    //set vertex parameters to extend the tree
    //setup new vertex(link to near)
    store[vnum].last = &store[index];
    store[vnum].self = result[rindex];
    
    //draw the tree
    //drawline(image, store[vnum].self,  store[vnum].last->self);
    
    
    cout<<"line"<<endl;
    cout<<"150"<<endl;
    cout<<store[vnum].self.x<<" "<<store[vnum].self.y<<endl;
    cout<<store[vnum].last->self.x<<" "<<store[vnum].last->self.y<<endl;
    cout<<"endline"<<endl;
    
    
    //setup near vertex(link to new)
    store[index].next[store[index].nextnum] = &store[vnum];
    vnum++;
    store[index].nextnum++;
 
    if(test.at(store[vnum-1].self.x,store[vnum-1].self.y) == 1){
      //   cout<<" Destination reached "<<endl;
      break;
    }

    if(vnum > 5999){
      //    cout<<" Exceeds memory "<<endl;
      break;
    }
  }

  cout<<"Path length = "<<4*traceback(test, store[vnum-1])<<endl;
  cout<<"Total searched distance = "<<4*(vnum-1)<<endl;
  cout<<"Total Time = "<<double(clock()-TotalTime)/CLOCKS_PER_SEC<<endl;

  return 0;

}
