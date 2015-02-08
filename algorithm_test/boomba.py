from math import *
import random

grid=[[0,0,0,0,1,0],
	  [0,1,0,0,0,0],
	  [0,0,0,1,0,0],
	  [1,0,0,0,1,0],
	  [0,0,1,0,0,0],
	  [1,0,0,0,1,0]]

motion=[[1,0],[-1,0],[0,-1],[0,1],[1,1],[1,-1],[-1,1],[-1,-1]]
#stores distance of detected obstacles from current position
#landmarks=[]
#spawn=[2,2]
odom_pose=[0,0]

#motion_name=['v','^','<','>','\\',]
movements=['right','down','left','down','down']
measurements=['obstacle','free','obstacle','free','free']


def Gaussian(self, mu, sigma, x):        
        # calculates the probability of x for 1-dim Gaussian with mean mu and var. sigma
	return exp(- ((mu - x) ** 2) / (sigma ** 2) / 2.0) / sqrt(2.0 * pi * (sigma ** 2))

def distance(point1,point2):
	return sqrt((point1[0]-point2[0])**2 + (point1[1]-point2[1])**2)

def update_landmarks(obstacles,point):
	landmarks=[]
	for i in obstacles:
		landmarks.append(distance(i,point))
	return landmarks

def detect_obstacle(grid,pose): #STATE VALIDITY CHECKER
	#print pose
	if pose[0] < 0 or pose [0] >=len(grid) or pose[1] <0 or pose[1] >= len(grid[0]):
		return True
	elif grid[pose[0]][pose[1]]==1:
		return True
	return False #means a valid state
	

#move randomly in a direction
	#if obstacle or end of map encountered
		#add odom_pose of robot to obstacles list
##this function keeps robot moving in a random direction until an obstacle is encountered
def update_obstacles(grid,pose):
	move=motion[int(random.random()*len(motion))] # select a random motion from list of motions
	while True:
		x=pose[0]+move[0]
		y=pose[1]+move[1]
		if detect_obstacle(grid,[x,y]) and [x,y] not in obstacles:
			if x >=-1 and x<=len(grid) and y>=-1 and y<=len(grid[0]):
				obstacles.append([x,y])
			break;			
		else:
			pose=[x,y]
			move=motion[int(random.random()*len(motion))]
		#break

def update_obstacle_map(obstacles,grid):
	map=[[' ' for row in range(len(grid[0])+2)] for col in range(len(grid)+2)]
	for i in obstacles:
		x=i[0]+1
		y=i[1]+1
		if x >=0 and x<len(map) and y>=0 and y<len(map[0]):
			map[x][y]='*'
	return map

def move(particles,grid):
	for p in particles:
		move=motion[int(random.random()*len(motion))]
		x=(p[0]+move[0])%len(grid)
		y=(p[1]+move[1])%len(grid[0])
	return particles

def initialize_particles(no_of_particles,grid):
	particles=[]
	for p in range(no_of_particles):
		x=int(random.random()*len(grid))
		y=int(random.random()*len(grid[0]))
		particles.append([x,y])
	return particles

obstacles=[]
start=[1,1]
print 'Boomba'
for i in range(250):
	update_obstacles(grid,[2,5])
for i in update_obstacle_map(obstacles,grid):
	print i
print update_landmarks(obstacles,[1,1])
# p=initialize_particles(15,grid)
# for i in update_obstacle_map(p,grid):
# 	print i
# p=move(p,grid)
# print '=========================================='
# for i in update_obstacle_map(p,grid):
# 	print i
# """
# Using Bayesian learning to make this model
# Particles are discrete beliefs spread throughout the state space.-> Prior
# Likelihood data is received from the sensor data.
# Using Likelihood,update prior to centre distribution around focussed particles.
# """