#include <getopt.h>
#include <sstream>
#include <iostream>
#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <pbf_input.hh>

using namespace std;

int main(int argc, char *argv[])
{
   //process command line options
   string sPath = "/home/dan/data/TEST";
   double dThreshold=0.;
   int module=-1,channel=-1;
   bool bPlotOnline=false;
   int c;
   stringstream tempss;
   while(1)  {
      static struct option long_options[] =
	{	   
	   {"file", required_argument, 0, 'f'},
	   {"threshold", required_argument, 0, 't'},
	   {"module", required_argument, 0, 'm'},
	   {"channel", required_argument, 0, 'c'},	   
	   {0,0,0,0}
	};

      int option_index = 0;
      c = getopt_long(argc, argv, "f:t:m:c:g",
		      long_options, &option_index);
      if(c==-1) break;

      switch(c) {	   
       case 'f':
	 sPath = optarg;
	 break;
       case 't':
	 tempss<<optarg;
	 tempss>>dThreshold;
	 break;
       case 'm':
	 tempss<<optarg;
	 tempss>>module;
	 break;
       case 'c':
	 tempss<<optarg;
	 tempss>>channel;
	 break;
       case 'b':
	 bPlotOnline=true;
	 break;
       default: 
	 break;
      }//end switch
      
   }//end while   
      

   pbf_input FileReader;
   if(FileReader.open_file(sPath)!=0)  {
      cout<<"Bad path"<<endl;
      return -1;
   }
   
   TApplication *theApp = new TApplication("App",&argc,argv);
   TCanvas *can = new TCanvas("can","can");
   //
   TH1F *spectrum = new TH1F("spectrum","",100100,-100,100100);
   //
   
   can->cd();

   while(FileReader.get_next_event()==0)  {
      int hEventNumber = FileReader.event_number();
      for(int x=0;x<FileReader.num_channels();x++)	{
	 char *data;
	 unsigned int datasize;
	 long long int dataTime;
       
	 for(int y=0;y<FileReader.num_data(x);y++){	      
	    	    
	    FileReader.get_data(x,y,data,datasize,dataTime);
	    u_int32_t *easierData = (u_int32_t*)data;
	    int hChannelID=0,hModuleID=0;
	    FileReader.channel_id(x,hChannelID,hModuleID);

	    if((channel!=-1 && module==-1 && channel!=hChannelID) ||
	       (channel!=-1 && module!=-1 && channel!=hChannelID && module!=hModuleID))
	      continue;
	    
	    int    bin=0;	    
	    int    lastBin=-1;
	    double baseline=0.;
	    double total=0.;
	    double lastTwo=0.;
	    int lowestBin=-1;
	    int lowestValue=100000;
	    for(unsigned int z=0;z<datasize/2;z+=2)  {	     
	      lastTwo=0.;
	      if(bin<10){
		baseline+=easierData[z/2]&0xFFFF;

		baseline+=(easierData[z/2]>>16)&0xFFFF;
	      }
	      //if(bin==4)
	      //baseline/=4.;
	      total+=easierData[z/2]&0xFFFF;
	      total+=(easierData[z/2]>>16)&0xFFFF;
	      lastTwo+=easierData[z/2]&0xFFFF;
	      lastTwo+=(easierData[z/2]>>16)&0xFFFF;
	      bin+=2;
	      lastBin=bin;

	      //other method
	      if((easierData[z/2]&0xFFFF)<lowestValue) { 
		lowestValue=(easierData[z/2]&0xFFFF);
		lowestBin=z;
	      }
	      if(((easierData[z/2]>>16)&0xFFFF)<lowestValue) {
		lowestValue=((easierData[z/2]>>16)&0xFFFF);
		lowestBin=z;
	      }
	    }	 	 
	    baseline/=10.;
	    //baseline=16386;
	    total-=((double)bin)*baseline;
	    //	    cout<<baseline<<endl;
	    //condition on last two bins being near baseline
	    //if(total<0 && TMath::Abs((lastTwo/2.)-baseline<100))
	    if(lowestBin>5 && lowestBin<lastBin-5){
	      double tot=0.;
	      for(unsigned int z=lowestBin-3;z<lowestBin+3;z++){
		tot+=easierData[z/2]&0xFFFF;
		tot+=(easierData[z/2]>>16)&0xFFFF;
	      }	    
	      if((tot-baseline*12)*-1.>30000)
		cout<<hEventNumber<<" NUMBER"<<" TOT: "<<tot<<" BL: "<<baseline<<endl;
	      spectrum->Fill((tot-baseline*12)*-1.);
	    }
	    //spectrum->Fill((total*-1.));	 
	 }	 
	 if(bPlotOnline && hEventNumber%10000==0){	      
	    spectrum->Draw();
	    can->Update();
	 }	 
      }
   }
   

   spectrum->Draw();
   can->Update();
   TFile *file = TFile::Open("lastGraph.root","RECREATE");
   can->Write();
   file->Close();
   
   //now loop though events
   return 0;
};
