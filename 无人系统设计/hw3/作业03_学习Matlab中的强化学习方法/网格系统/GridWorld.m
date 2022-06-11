function env = GridWorld 
% BASICGRIDWORLD: Creates basic gridworld RL environment.

% construct basic gridworld environment
GW = createGridWorld(5,5);

% set current, terminal states and obstacle states
GW.TerminalStates = "[5,3]";
GW.ObstacleStates = ["[2,3]";"[3,3]";"[4,2]";"[4,3]"];

% update gridworld
updateStateTranstionForObstacles(GW)

% set jump rule
GW.T(state2idx(GW,"[3,2]"),:,:) = 0;
GW.T(state2idx(GW,"[3,2]"),state2idx(GW,"[3,4]"),:) = 1;

% set rewards
nS = numel(GW.States);
nA = numel(GW.Actions);
GW.R = -1*ones(nS,nS,nA);
GW.R(state2idx(GW,"[3,2]"),state2idx(GW,"[3,4]"),:) = 5;
GW.R(:,state2idx(GW,GW.TerminalStates),:) = 10;

env = rlMDPEnv(GW);


    
    
            

    
