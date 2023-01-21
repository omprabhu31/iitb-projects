param T;
param nNodes;

set k := {1..T by 1};
set k1 := {1..(T-1) by 1};
set k2 := {0..T by 1};
set centers := {1..nNodes};

param Cg {centers};
param Ca {centers};
param SchDept{centers, k};
#param SchArriv{centers, k}; 

param Capacity{centers, k};
param DeptLim{centers, k};
param ArrivLim{centers, k};
param TrafLim{centers,centers,k}:=300;
param x{centers, k2};

# Control Variables
var Beta{centers, centers, k2} >=0.05, <=1;
var A{centers, k2} >=0 integer;
var D{centers, k2} >=0 integer;

# Cost Function
minimize z: (sum{i in centers, q in k} Cg[i]*(SchDept[i,q] - D[i,q])) + (sum{i in centers, q in k} Ca[i]*(sum{j in centers}(Beta[j,i,q-1]*x[j,q-1])- A[i,q]));

# Constraints
s.t. Balance_Constraint {i in centers, q in k1}: x[i,q+1] = x[i,q] - sum{b in centers:b!=i} (Beta[i,b,q]*x[i,q]) + sum{c in centers: c!=i} (Beta[c,i,q]*x[c,q]) - A[i,q] + D[i,q];
s.t. Departures_constraint {i in centers, q in k1}: sum{j in centers}(Beta[i,j,q+1]*x[i,q+1]) = D[i,q];
s.t. Arrival_constraint {i in centers, q in k}: sum{j in centers}(Beta[j,i,q-1]*x[j,q-1]) >= A[i,q];
s.t. Capacity_Constraint {i in centers, q in k}: D[i,q] + A[i,q] <= Capacity[i,q];
s.t. DeptLim_Constraint {i in centers, q in k}: D[i,q] <= DeptLim[i,q];
s.t. ArrivLim_Constraint {i in centers, q in k}: A[i,q] <= ArrivLim[i,q];
s.t. Traffic_Flow_Limit {i in centers, j in centers, q in k:j!=i}: Beta[i,j,q]*x[i,q] + Beta[j,i,q]*x[j,q] <= TrafLim[i,j,q];
#s.t. Initial_condition1 {i in centers}: D[i,0] = 0;
#s.t. Initial_condition2 {i in centers}: A[i,0] = 20;
#s.t. Initial_condition3 {i in centers, j in centers}: Beta[i,j,0] = 0;
#s.t. Beta_Constraint {i in centers, q in k}: sum{c in centers} (Beta[i,c,q]) = 1;