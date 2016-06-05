#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define max(a,b) (((a)>(b))?(a):(b))


long file_lines(char*);
void get_ratios(double*, double*, long, long, double*, double*, double*);
void get_temperature(char*, char*, double*, double*, double*, double*, long*, long*);
void get_feh(char*, char*, double, double, double, double, double*, double*, long*);


void get_ratios(double lares[], double ewares[],long na, long nf, double line1[], double line2[], double lratios[]){
	long i,li,lj,j;
	li=-1;
	lj=-1;

	for (i=0; i < nf; i++){	
		li=-1;
		lj=-1;
		for (j=0; j < na; j++)	{
			if (fabs(lares[j]-line1[i]) < 0.1)	li=j;
			if (fabs(lares[j]-line2[i]) < 0.1)	lj=j;
		}
		if (li == -1 || lj == -1) 
                        lratios[i]=-1; 
                else {
                        lratios[i]=ewares[li]/ewares[lj];
                        /*printf(" ratio: %f  -  %f  \n", line1[i], line2[i]);*/
                }
	}
}

long file_lines(char * file) {
    FILE * pFile;
	long n = 0;
	char str[200];
	pFile = fopen (file,"rt");
	if (pFile==NULL) perror ("Error opening file");
		else {
			while (!feof(pFile)) {
				fgets (str , 200, pFile);
				n++;
			}
		   	fclose (pFile);
		}
	return n-1;
}




void get_temperature(char* filecal,char* fileares, double* Teffout, double* erTeffout1, double* erTeffout2, double* erTeffout3, long* nout, long* nindout) {


/*Leitura do ficheiro de calibraçao*/
	char str[200];
	long nl;
	FILE * pFile;

	pFile = fopen(filecal,"rt");
	nl=file_lines(filecal);
	nl=nl-1;
	long i;
	char * pch;

	int stddev[nl];
/*	int nf[nl],ni[nl],nstars[nl];*/
	double line1[nl],line2[nl],nfit[nl];
        double rata[nl],ratb[nl],meanrat[nl];
/*	double nf[nl],ni[nl],teffa[nl],teffb[nl],rata[nl],ratb[nl],meanrat[nl]*/


	double calib[nl][4];
	char ele1[nl][6],ele2[nl][6];

/*primeira linha do ficheiro nao interessa*/
	fgets (str,200,pFile);

	for (i=0;i<nl;i++)
	{
		fgets (str,200,pFile);
		pch = strtok (str," ");
/*		nf[i]=atoi(pch);*/

		pch = strtok (NULL," ");
/*		ni[i]=atoi(pch);*/

		pch = strtok (NULL," ");
		stddev[i]=atoi(pch);

		pch = strtok (NULL," ");
/*		nstars[i]=atoi(pch);*/

		pch = strtok (NULL," ");
		line1[i]=atof(pch);

		pch = strtok (NULL," ");
		line2[i]=atof(pch);

		pch = strtok (NULL," ");
		strcpy (ele1[i],pch);

		pch = strtok (NULL," ");
		strcpy (ele2[i],pch);

		pch = strtok (NULL," ");
		calib[i][0]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][1]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][2]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][3]=atof(pch);

		pch = strtok (NULL," ");
/*		teffa[i]=atof(pch);*/

		pch = strtok (NULL," ");
/*		teffb[i]=atof(pch);*/

		pch = strtok (NULL," ");
		rata[i]=atof(pch);

		pch = strtok (NULL," ");
		ratb[i]=atof(pch);

		pch = strtok (NULL," ");
		meanrat[i]=atof(pch);

		pch = strtok (NULL," ");
		nfit[i]=atof(pch);

	}
        fclose(pFile);

/*leitura das medidas de ares*/
	long na;
	FILE * pFile2;

	pFile2 = fopen (fileares,"rt");
	na=file_lines(fileares);


	double lares[na],ewares[na];

/*primeira linha do ficheiro nao interessa*/
	for (i=0;i<na;i++)	{
		fgets (str,200,pFile2);
		pch = strtok (str," ");
		lares[i]=atof(pch);
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		ewares[i]=atof(pch);
	}
        fclose(pFile2);


/*Calcular os ratios:*/
	double lratios[nl];
	get_ratios(lares, ewares, na, nl, line1, line2, lratios);

	double teff[nl],teff1[nl],teff2[nl],stddev1[nl],stddev2[nl];/*,lratios1[nl];*/
	char ele1_1[nl][6],ele1_2[nl][6],ele2_1[nl][6],ele2_2[nl][6],eleind1[nl][6],eleind2[nl][6];
	long n1=0;
	long n2=0;

	for(i=0;i<nl;i++) {
		if (nfit[i] >4)	{
			if (lratios[i] < 0) lratios[i]=0.001;
			teff[i]=calib[i][0]+calib[i][1]*log10(lratios[i])+calib[i][2]*log10(lratios[i])*log10(lratios[i])+calib[i][3]*log10(lratios[i])*log10(lratios[i])*log10(lratios[i]);
		} else {
                        if (nfit[i] >2) lratios[i]=1./lratios[i];
			teff[i]=calib[i][0]+calib[i][1]*lratios[i]+calib[i][2]*lratios[i]*lratios[i]+calib[i][3]*lratios[i]*lratios[i]*lratios[i];
		}
		if (teff[i] >=3800. && teff[i] <=6800. && lratios[i] >=0.01 && lratios[i] <=100) {
/*		if (teff[i] >=3800. && teff[i] <=6800. && lratios[i] >=rata[i] && lratios[i] <=ratb[i]) {*/
			teff1[n1]=teff[i];
			stddev1[n1]=stddev[i];
/*			lratios1[n1]=lratios[i];*/
			strcpy (ele1_1[n1],ele1[i]);
			strcpy (ele2_1[n1],ele2[i]);
			n1++;
		}
	}

	double sigma1=0.,soma=0.,meanteff=0.;
	soma=0;
	for(i=0;i<n1;i++)	soma+=teff1[i];
	meanteff=soma/n1;
	soma=0;
	for(i=0;i<n1;i++)	soma+=(teff1[i]-meanteff)*(teff1[i]-meanteff);
	sigma1=sqrt (soma/(n1-1));

	for(i=0;i<n1;i++) {
/*                printf(" ratio %i - Teff: %f  - meanteff: %f -  sigma1: %f: condition: %d\n",i,teff1[i],meanteff,sigma1, abs(teff1[i]-meanteff) <= 2.*sigma1);*/
		if (abs(teff1[i]-meanteff) <= 2.*sigma1 ) {
			teff2[n2]=teff1[i];
			stddev2[n2]=stddev1[i];
			strcpy (ele1_2[n2],ele1_1[i]);
			strcpy (ele2_2[n2],ele2_1[i]);
			n2++;
		}
	}
	double sigma2=0.,meanteff2=0.;
	soma=0;
	for(i=0;i<n2;i++)	soma+=teff2[i];
	meanteff2=soma/n2;
	soma=0;
	for(i=0;i<n2;i++)	soma+=(teff2[i]-meanteff2)*(teff2[i]-meanteff2);
	sigma2=sqrt (soma/(n2-1));

	soma=0;
	double soma2=0;
	for(i=0;i<n2;i++) {
		soma+=teff2[i]/(stddev2[i]*stddev2[i]);
		soma2+=1./(stddev2[i]*stddev2[i]);
	}
	double Teff_final,erTeff;
	int flag;
	long nrind=1,j;
	strcpy (eleind1[0],ele1_2[0]);
	strcpy (eleind2[0],ele2_2[0]);

	for(i=0;i<n2;i++) {
		flag=0;
		for (j=0;j<nrind;j++) {
			if ( (strcmp(eleind1[j],ele1_2[i]) == 0 && strcmp(eleind2[j],ele2_2[i]) == 0) || (strcmp(eleind1[j],ele2_2[i]) == 0 && strcmp(eleind2[j],ele1_2[i]) ==0 )) {
				flag=1;
			}
		}
		if (flag == 0) {
			strcpy (eleind1[nrind],ele1_2[i]);
			strcpy (eleind2[nrind],ele2_2[i]);
			nrind++;
		}
	}

	/*
	Correction for stars hotter than 6000 K
	(teffvecw(hteff)-res(0))/res(1)
	res0       3191.1655
	res1      0.47104218
	*/


	Teff_final=soma/soma2;
	erTeff=sigma2;
	double Teff_final_cor;
	if (Teff_final > 6000) Teff_final_cor=(Teff_final-3191.1655)/0.47104218; else Teff_final_cor=Teff_final;

	if (Teff_final > 6000)
	{	
		printf("Correction for stars hooter than 6000 K:\n");
		printf("Teff Corrected: %f +- %f\n",Teff_final_cor,erTeff/sqrt(nrind));

	if (Teff_final_cor < 6000)
	{
		printf("Correction invalid for this star because Teff_cor < 6000 K.\n");
		Teff_final_cor=Teff_final;
	}
	}
	printf("Final Teff: %f +- %f   -  nratios: %d\n",Teff_final_cor,erTeff/sqrt(nrind), n2);

	*Teffout=Teff_final_cor;
	*erTeffout1=erTeff/sqrt(nrind);
	*erTeffout2=erTeff;
	*erTeffout3=erTeff/sqrt(n2);
	*nout=n2; 
	*nindout=nrind;
	return;
}

void get_feh(char* filecal, char* fileares, double teff, double erteff1, double erteff2, double erteff3, double* fehout, double* erfehout, long* nout){

/*Leitura do ficheiro de calibraçao*/
	char str[200];
	long nl;
	FILE * pFile;
	pFile = fopen (filecal,"rt");
	nl=file_lines(filecal);
	nl=nl-1;

	long i, j;
	char * pch;
	/*int ni[nl],nstars[nl];*/
	double stddev[nl],line[nl],teffa[nl],teffb[nl],feha[nl],fehb[nl],ewa[nl],ewb[nl];
	char ele[nl][6];
	double calib[nl][6];

/*primeira linha do ficheiro nao interessa*/
	fgets (str,400,pFile);

	for (i=0;i<nl;i++) {
		fgets (str,400,pFile);

		pch = strtok (str," ");
/*		ni[i]=atoi(pch);*/

		pch = strtok (NULL," ");
		stddev[i]=atof(pch);

		pch = strtok (NULL," ");
/*		nstars[i]=atoi(pch);*/

		pch = strtok (NULL," ");
		line[i]=atof(pch);

		pch = strtok (NULL," ");
		strcpy (ele[i],pch);

		pch = strtok (NULL," ");
		calib[i][0]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][1]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][2]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][3]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][4]=atof(pch);

		pch = strtok (NULL," ");
		calib[i][5]=atof(pch);

		pch = strtok (NULL," ");
		teffa[i]=atof(pch);

		pch = strtok (NULL," ");
		teffb[i]=atof(pch);

		pch = strtok (NULL," ");
		feha[i]=atof(pch);

		pch = strtok (NULL," ");
		fehb[i]=atof(pch);

		pch = strtok (NULL," ");
		ewa[i]=atof(pch);

		pch = strtok (NULL," ");
		ewb[i]=atof(pch);
	}
        fclose(pFile);
        

/*leitura das medidas de ares*/
	long na;
	FILE * pFile2;
	pFile2 = fopen (fileares,"rt");
	na=file_lines(fileares);
	double lares[na],ewares[na];

/*primeira linha do ficheiro nao interessa*/
	for (i=0;i<na;i++)	{
		fgets (str,200,pFile2);
		pch = strtok (str," ");
		lares[i]=atof(pch);
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		pch = strtok (NULL," ");
		ewares[i]=atof(pch);
	}
        fclose(pFile2);


/*Obter as EW para as riscas que se tem...*/
	double ewaresin[nl];
	for (i=0;i<nl;i++) {
		ewaresin[i]=-1;
		for (j=0;j<na;j++) {
			if (fabs(lares[j]-line[i]) < 0.1){
				ewaresin[i]=ewares[j];
			}
		}
	}

/*Calcular FE/H:*/
	double feh[nl],feh1[nl],feh2[nl],stddev1[nl],stddev2[nl];
	long n1=0;
	long n2=0;

/*	printf("[Fe/H]= %18.12f + %18.12f *EW + %18.12f *Teff + %18.12f *EW*EW + %18.12f * Teff*Teff\n", calib[i][0] , calib[i][1], calib[i][2], calib[i][3], calib[i][4]);*/
	for(i=0;i<nl;i++) {
		float va;
		float vb;
		float vc;
		va= calib[i][3];
		vb= (calib[i][1] + calib[i][5] * teff);
		vc= (calib[i][0] + calib[i][2]*teff + calib[i][4]*teff*teff - ewaresin[i]);
/*		feh[i]=calib[i][0]+calib[i][1]*ewaresin[i]+calib[i][2]*teff+calib[i][3]*ewaresin[i]*ewaresin[i]+calib[i][4]*teff*teff;*/
		if (vb*vb < 4*va*vc)
			feh[i]=-101.;
		else
			feh[i]= (-vb + sqrt(vb*vb - 4*va*vc) )/ (2*va) ;
		if ( (ewaresin[i] > ewa[i]-0.  && ewaresin[i] < ewb[i]+0.) && (teff >= teffa[i]-100.  && teff <= teffb[i]+100. ) &&  ( feh[i] >= feha[i]-0. && feh[i] <= fehb[i]+0.  )   )	{
			feh1[n1]=feh[i];
			stddev1[n1]=stddev[i];
			n1++;
		}
	}

	double sigma1=0.,soma=0.,meanfeh=0.;
	soma=0;
	for(i=0;i<n1;i++)	soma+=feh1[i];
	meanfeh=soma/n1;
	soma=0;
	for(i=0;i<n1;i++)	soma+=(feh1[i]-meanfeh)*(feh1[i]-meanfeh);
	sigma1=sqrt (soma/(n1-1));

	for(i=0;i<n1;i++) {
		if (abs(feh1[i]-meanfeh) <= 2.*sigma1) {
			feh2[n2]=feh1[i];
			stddev2[n2]=stddev1[i];
			n2++;
		}
	}

	double sigma2=0.,meanfeh2=0.;
	soma=0;
	for(i=0;i<n2;i++)	soma+=feh2[i];
	meanfeh2=soma/n2;
	soma=0;
	for(i=0;i<n2;i++)	soma+=(feh2[i]-meanfeh2)*(feh2[i]-meanfeh2);
	sigma2=sqrt (soma/(n2-1));

	soma=0;
	double soma2=0;
	for(i=0;i<n2;i++) {
		soma+=feh2[i]/(stddev2[i]*stddev2[i]);
		soma2+=1./(stddev2[i]*stddev2[i]);
	}

	double Feh_final,erFeh;
	Feh_final=soma/soma2;
	erFeh=sigma2;

	*fehout=Feh_final;
	*erfehout=erFeh;
	*nout=n2;
	return;
}





