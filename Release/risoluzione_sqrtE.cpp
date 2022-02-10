// .L NuovaMacroEsempio.c+
//> run()

#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TRandom.h"
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TSpectrum.h"
using namespace std;
vector<string> run(double percE,double risA,string lineX17,string lineIPC){
    int dim=1000000;
    vector<double> em(dim), ep(dim), massa(dim),invMass(dim);
    vector<double> em0(dim), ep0(dim), massa0(dim),invMass0(dim);
    vector<double> angleE0(dim),Y0(dim),angleE(dim),Y(dim);
    //    line="risoluzioneIPC_1m_30-300.txt";//risoluzioneX17
    ifstream myfile(lineIPC);
    int ind=0;
    double em_v,ep_v,massa_v,angleE_v,Y_v;
    if (myfile.is_open())
    {
        //        pos.x()         <<" "
        //        <<   pos.z()         <<" "
        //        <<   x_saved         <<" "
        //        <<   z_saved
        while (myfile >> em_v >> ep_v >> massa_v >> massa_v  >> angleE_v >> Y_v)
        {
            if(em_v!=0)
            {
                em[ind]=em_v;
                ep[ind]=ep_v;
                massa[ind]=massa_v;
                angleE[ind]=angleE_v;
                Y[ind]=Y_v;
                ind++;
            }
        }
        myfile.close();
    }
    
    ofstream myfileout;
    auto str1 = std::to_string(percE);
    auto str2 = std::to_string(risA);
    string lineout="invmass_IPC_"+str1+str2+".txt";
    myfileout.open (lineout);
    //applico la risoluzione
    double risA_rad=M_PI/180*risA;
    risA=risA*sqrt(2.);//
    cout<<ind<<endl;
    for(int i=0;i<ind;i++)
    {
        percE=8/sqrt(em[i]);
        sigma=percE/2.35;
        double em_r=gRandom->Gaus(em[i],percE*em[i]);
        double ep_r=gRandom->Gaus(ep[i],percE*ep[i]);
        double phi_r=gRandom->Gaus(angleE[i],risA_rad);
        //        if(i<10)cout<<phi_r_old<<"->"<<phi_r<<";"<<endl;
        
        double mom_plus =sqrt(pow(em_r,2)-pow(massa[i],2));
        double mom_minus=sqrt(pow(ep_r,2)-pow(massa[i],2));
        invMass[i]=sqrt(pow(massa[i],2)+pow(massa[i],2)+
                        2*(em_r*ep_r-mom_plus*mom_minus*cos(phi_r)));
        myfileout << invMass[i] << "\n";
    }
    
    myfileout.close();
    
    //    line="risoluzioneX17_1m_30-300.txt";//risoluzioneX17
    ifstream myfile2(lineX17);
    int ind0=0;
    if (myfile2.is_open())
    {
        //        while (myfile2 >> em_v >> ep_v >> massa_v >> massa_v >> phi_v)
        while(myfile2 >> em_v >> ep_v >> massa_v >> massa_v >> angleE_v >> Y_v)
        {
            if(em_v!=0)
            {
                em0[ind0]=em_v;
                ep0[ind0]=ep_v;
                massa0[ind0]=massa_v;
                angleE0[ind0]=angleE_v;
                Y0[ind0]=Y_v;
                ind0++;
            }
            
        }
        myfile2.close();
    }
    
    cout<<ind0<<endl;
    ofstream myfileout2;
    string lineout2="invmass_X17_"+str1+str2+".txt";
    myfileout2.open (lineout2);
    //applico la risoluzione
    
    for(int i=0;i<ind0;i++)
    {
        double em_r=gRandom->Gaus(em0[i],percE*em0[i]);
        double ep_r=gRandom->Gaus(ep0[i],percE*ep0[i]);
        double phi_r=gRandom->Gaus(angleE0[i],risA_rad);
        
//        if(i<10)cout<<risA_rad<<"->"<<phi_r<<";"<<angleE0[i]<<endl;
        double mom_plus =sqrt(pow(em_r,2)-pow(massa0[i],2));
        double mom_minus=sqrt(pow(ep_r,2)-pow(massa0[i],2));
        invMass0[i]=sqrt(pow(massa0[i],2)+pow(massa0[i],2)+
                         2*(em_r*ep_r-mom_plus*mom_minus*cos(phi_r)));
        myfileout2 << invMass0[i] << "\n";
    }
    
    myfileout2.close();
    
    vector<string> strVec = {lineout, lineout2};
    ofstream myfileout1;
    string lineout1="YvsAngle_IPC.txt";
    myfileout1.open (lineout1);
    for(int i=0;i<ind;i++)
    {
        myfileout1 << Y[i] <<" "<< angleE[i] << "\n";
    }
    
    ofstream myfileout3;
    string lineout3="YvsAngle_X17.txt";
    myfileout3.open (lineout3);
    for(int i=0;i<ind0;i++)
    {
        myfileout3 << Y0[i] <<" "<< angleE0[i] << "\n";
    }
    myfileout1.close();
    myfileout3.close();
    return strVec;
}



