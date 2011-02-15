#include "use_timer.h"

void usetimer::start(){
  for (int i = 0; i<10; i++){
    time_measurements[i] = 0;
  }
  return;
}

void usetimer::reset() {
  timer.reset();
  return;
}

void usetimer::measurements(int n){
  time_measurements[n] = (int) timer.getTime();
  return;
}

void usetimer::getTimeAndReset(int n){
  time_measurements[n] = (int) timer.getTimeAndReset();
  return;
}

void usetimer::add(int n){
  time_measurements[n] += (int) timer.getTimeAndReset();
  return;
}

void usetimer::zero(int n){
  time_measurements[n] = 0;
  return;
}

void usetimer::output() {
  int totaltime;
  for (int i = 0; i<10; i++){
    totaltime += time_measurements[i];
  }
  //format is valid for measurements up to ca. 3 hours
  printf("\n");
  printf("**************************************************************\n");
  printf("total runtime:\t\t%i\n\n",  totaltime );
  printf("Times:\t\ttot\t\tavg\t\tsite\tperc\n");
  
  if ((*parameters).get_startcondition()==START_FROM_SOURCE){
    printf("sourc:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[0], time_measurements[0], time_measurements[0]/VOL4D, (float)time_measurements[0]/totaltime*100 );
  }
  else printf("Init.:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[0], time_measurements[0], time_measurements[0]/VOL4D,(float)time_measurements[0]/totaltime*100 );
  if((*parameters).get_startcondition()!=START_FROM_SOURCE)
    printf("therm:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[1], time_measurements[1]/(*parameters).get_thermalizationsteps() , time_measurements[1]/(*parameters).get_thermalizationsteps()/VOL4D , (float)time_measurements[1]/totaltime*100 );
  
  printf("plaq.:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[2], time_measurements[2]/(*parameters).get_heatbathsteps(), time_measurements[2]/(*parameters).get_heatbathsteps()/VOL4D, (float) time_measurements[2]*100/totaltime);
  printf("poly.:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[3], time_measurements[3]/(*parameters).get_heatbathsteps()/4 , time_measurements[3]/(*parameters).get_heatbathsteps()/4/VOL4D , (float) time_measurements[3]*100/totaltime);
  printf("updte:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[4] ,(time_measurements[4]/(*parameters).get_heatbathsteps() ) , (time_measurements[4]/(*parameters).get_heatbathsteps() )/VOL4D 
  	, (float) (time_measurements[4])*100/totaltime);
  printf("overr:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[5] ,(time_measurements[5]/(*parameters).get_heatbathsteps()) , (time_measurements[5]/(*parameters).get_heatbathsteps()) /VOL4D
  	, (float) (time_measurements[5])*100/totaltime);
  
  printf("\nuptot:\t%12i\t%12i\t%12i\t%.3f\n", time_measurements[4] + time_measurements[1],(time_measurements[4]/(*parameters).get_heatbathsteps() + time_measurements[1]/(*parameters).get_thermalizationsteps()) , (time_measurements[4]/(*parameters).get_heatbathsteps() + time_measurements[1]/(*parameters).get_thermalizationsteps())/VOL4D
  	, (float) (time_measurements[4] + time_measurements[1])*100/totaltime);
  printf("**************************************************************\n");
 
  //save some data to file
  ofstream out;
  stringstream str_filename;
  str_filename<<"time_"<<NTIME<<"_"<<NSPACE<<"_";
#ifdef _RECONSTRUCT_TWELVE_
  str_filename<<1;
#else
  str_filename<<0;
#endif
  out.open(str_filename.str().c_str()); 
  if (out.is_open())
  {
    out << NTIME << "\t" << NSPACE << "\t" << totaltime << "\t" << time_measurements[0] << "\t" <<  time_measurements[1]/(*parameters).get_thermalizationsteps() << "\t" << time_measurements[2]/(*parameters).get_heatbathsteps()<< "\t" << time_measurements[3]/(*parameters).get_heatbathsteps()/4 << "\t" << (time_measurements[4]/(*parameters).get_heatbathsteps() ) << "\t" << (time_measurements[5]/(*parameters).get_heatbathsteps())<< endl;
    out.close();
  }
  else cout << "Unable to open file for output" << endl;
  return;
}
