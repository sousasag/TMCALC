#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lineRatioCalibCore.h"
#define max(a,b) (((a)>(b))?(a):(b))


int main(int argc, char * argv[])
{

	if (argc != 4) {
		system("clear");
		printf("ERROR 1; Need 3 files\n");
		printf("Usage Example:\n");
		printf("getTeffFeh ratios_list.dat feh_calib_lines.dat /home/sousasag/posdoc/line_ratios/ARES_EW_HARPS/HD20619.ares\n");
		printf("./getTeffFeh ratios_list.dat feh_calib_lines.dat /home/sousasag/posdoc/line_ratios/ARES_EW_HARPS/HD20619.ares\n");
		return 0;
	}

	char filecalteff[200],filecalfeh[200],fileares[200];

	strcpy(filecalteff,argv[1]);
	strcpy(filecalfeh,argv[2]);
	strcpy(fileares,argv[3]);

	double teff, erteff1, erteff2, erteff3, feh, erfeh;
	long nteff, nfeh, nind;

	get_temperature(filecalteff, fileares, &teff, &erteff1, &erteff2, &erteff3, &nteff, &nind);

	double fehmax, fehmin;

	get_feh(filecalfeh, fileares, teff+erteff1, erteff1, erteff2, erteff3, &feh, &erfeh,&nfeh);
	fehmax=feh;
	get_feh(filecalfeh, fileares, teff-erteff1, erteff1, erteff2, erteff3, &feh, &erfeh,&nfeh);
	fehmin=feh;
	double erfehtot=(fehmax-fehmin)/2.;
	/*printf("MajFeh: %f - %f - %f \n", erfehtot, fehmax, fehmin);*/


	get_feh(filecalfeh, fileares, teff, erteff1, erteff2, erteff3, &feh, &erfeh,&nfeh);

	printf("starfile: %s\n",fileares); 
	printf("number of ratios used for Teff: %li with stddev %f\n",nteff,erteff2);
	printf("Teff: %f +- %f (assuming the standard deviation)\n",teff,erteff2);
	printf("Teff: %f +- %f (assuming the standard deviation and the number of independent ratios (%li))\n",teff,erteff1,nind);
	printf("Teff: %f +- %f (assuming the standard deviation and that all ratios used are independent (%li)) \n",teff,erteff3,nteff);

	printf("number of lines used for [Fe/H]: %li with stddev %f\n",nfeh,erfeh);
	printf("[Fe/H]: %f +- %f (assuming the standard deviation)\n",feh,erfeh);
	printf("[Fe/H]: %f +- %f (assuming the standard deviation and that all lines used are independent (%li)) \n",feh,erfeh/sqrt(nfeh),nfeh);
	double erfehtot1=sqrt(erfehtot*erfehtot+erfeh*erfeh);
	double erfehtot2=sqrt(erfehtot*erfehtot+erfeh/sqrt(nfeh)*erfeh/sqrt(nfeh));
	printf("[Fe/H]: %f +- %f (assuming the standard deviation and error in Teff)\n",feh,erfehtot1);
	printf("[Fe/H]: %f +- %f (assuming the standard deviation and that all lines used are independent and error in Teff(%li)) \n",feh,erfehtot2, nfeh);
/*Correction for [Fe/H]: [Fe/H]_cor=[Fe/H]- (-0.00649147 - 0.124963 * [Fe/H] -0.0962114* [Fe/H]^2) */
/*	double fehcor=feh- (-0.00806967 - 0.126598 * feh -0.0916202*feh*feh);*/
/*	double fehcor=feh- (0.00141308 - 0.0907054 * feh -0.204774*feh*feh - 0.128857*feh*feh*feh);*/
	double meanfehconst=0.0126148; /*determined at -0.2 by looking at the plot of the feh dependencies */
	double fehcor=0;	
	if (feh < meanfehconst -0.2)
		fehcor=feh-meanfehconst;
	else {
		fehcor=feh- (-0.000726960 -0.0950966 * feh -0.128329*feh*feh);
	}
	printf("[Fe/H] Corrected: %f +- %f (with small correction dependent on the derived [Fe/H]) \n",fehcor,erfehtot1);

return 0;
}
