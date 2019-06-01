#include "experiment.hpp"

bool Experiment::start()
{

    cout<<"Initialize experiment data"<< endl;

    Match match=Match(gconst.TURNS,1, gconst);
    match.init();

    srand (time(NULL));
    Graph3D ogregraph;
    ogregraph.initApp();
    ogregraph.gconst=gconst;

    ogregraph.InitSphereList(match.players, match.constraints);

    numElements = gconst.number_of_bots;

    cout<<"Initialize CUDA data"<< endl;

    Player *hbotList;
    Player *dbotList;
    size_t botListSize = numElements * sizeof(Player);
    hbotList = (Player*)malloc(botListSize);
    cudaMalloc((void**)&dbotList, botListSize);


    GameConstants *hconst;
    GameConstants *dconst;
    size_t constSize = sizeof(GameConstants);
    hconst = (GameConstants*)malloc(constSize);
    cudaMalloc((void**)&dconst, constSize);

    ::memcpy(hconst, &gconst, constSize);
    //hconst=&gconst;
    //hconst->DEPTH =gconst.DEPTH;
    //hconst->WIDTH =gconst.WIDTH;
    //hconst->HEIGHT=gconst.HEIGHT;
    gpuAssert(cudaMemcpy(dconst, hconst, constSize, cudaMemcpyHostToDevice));


    int constrSize=(int)(match.constraints.size());
    Constraint *hConstrainList;
    Constraint *dConstrainList;
    size_t ConstrainListSize =  constrSize* sizeof(Constraint);
    hConstrainList = (Constraint*)malloc(ConstrainListSize);
    cudaMalloc((void**)&dConstrainList, ConstrainListSize);

    int allconstrSize=(int)(match.allconstraints.size());
    Constraint *hallConstrainList;
    Constraint *dallConstrainList;
    size_t allConstrainListSize =  allconstrSize* sizeof(Constraint);
    hallConstrainList = (Constraint*)malloc(allConstrainListSize);
    cudaMalloc((void**)&dallConstrainList, allConstrainListSize);

    ::memcpy(hallConstrainList, match.allconstraints.data(), allConstrainListSize);
    cudaMemcpy(dallConstrainList, hallConstrainList, allConstrainListSize, cudaMemcpyHostToDevice);

    ::memcpy(hbotList, match.players.data(), botListSize);
    gpuAssert(cudaMemcpy(dbotList, hbotList, botListSize, cudaMemcpyHostToDevice));

    initKernelParticals( dbotList, gconst.number_of_bots, dconst);


    ::memcpy(hConstrainList, match.constraints.data(), ConstrainListSize);
    gpuAssert(cudaMemcpy(dConstrainList, hConstrainList,  ConstrainListSize, cudaMemcpyHostToDevice));


    thrust::host_vector<int> D1(10);
    thrust::fill(D1.begin(), D1.begin() + 7, 9);
    //initTrust(D1);

    cout<<"Start experiment"<< endl;

    for(int tick=0; tick<match.max_turns; tick++)
        {

            ogregraph.MoveSphere(match.players, match.constraints,match.allconstraints);
            ogregraph.DrawSphere(match.players, match.constraints);
            ogregraph.showInfoWidget(tick);
            ogregraph.getRoot()->renderOneFrame();

            if(ogregraph.gconstChange )
            {
                ogregraph.gconstChange=false;
                gconst= ogregraph.gconst;
                ::memcpy(hconst, &gconst, constSize);
                //hconst=&gconst;
                gpuAssert(cudaMemcpy(dconst, hconst, constSize, cudaMemcpyHostToDevice));
            }

            //graphModule.KeyDirection=0;
            clock_t startTimer = clock();

            updateBallVelocity(dbotList, dallConstrainList, dconst, gconst.number_of_bots);

            if(tick>45)
            {
                updateConstrains(dbotList, dConstrainList, dallConstrainList, dconst, gconst.number_of_bots, constrSize);
            }

            gpuAssert(cudaMemcpy(hbotList, dbotList, botListSize, cudaMemcpyDeviceToHost));
            std::memcpy(match.players.data(), hbotList, botListSize);

            gpuAssert(cudaMemcpy(hConstrainList, dConstrainList, ConstrainListSize, cudaMemcpyDeviceToHost));
            std::memcpy(match.constraints.data(), hConstrainList, ConstrainListSize);

            gpuAssert(cudaMemcpy(hallConstrainList, dallConstrainList, allConstrainListSize, cudaMemcpyDeviceToHost));
            std::memcpy(match.allconstraints.data(), hallConstrainList, allConstrainListSize);

           // sleep(1);
            clock_t stopTimer = clock();
            timerLoop = (int)(stopTimer - startTimer);

            if(ogregraph.exitGraphLoop)
                break;

        }

    cout<<"Free experiment data"<< endl;
    cudaFree(dallConstrainList);
    cudaFree(dConstrainList);
    cudaFree(dbotList);
    cudaFree(dconst);

    free(hallConstrainList);
    free(hConstrainList);
    free(hbotList);

    free(hconst);

    cout<<"Finish experiment"<< endl;
    result_exp=true;
    return result_exp;

}
