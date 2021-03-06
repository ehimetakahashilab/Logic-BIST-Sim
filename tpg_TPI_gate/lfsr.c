#include <stdio.h>
#include <stdlib.h>

#define BUF_MAX 20
#define PHASE_SHIFT 0
#define LTRTPG 0 //1:test pattern generated by LTRTPG 
#define K 3 //k value of LTRTPG

void phase_shifter(int Y[], int nfin);
void LT_RTPG(int X[],int RTPG[],int TFF[],int nfin);

int main(int argc, char *argv[]){
  int i,ia, ib, ic,max, test_vec, nfin;
  int XOR[10];
  int *X,*X_LOOP;
  int loop,nfin_max,tmp_fin;
  char buf[BUF_MAX];
  FILE *fin, *fout;

  if(argc < 3){
    printf("error: too less arguments!\n");
    exit(1);
  }

  fin = fopen(argv[1], "r");
  if(fin == NULL){
    printf("error: '%s' is not found\n", argv[1]);
    exit(1);
  }
  fscanf(fin, "%d %d %d %d %d %d", &nfin, &nfin, &nfin, &nfin, &nfin, &nfin);
  fclose(fin);

  fin = fopen("lfsr.dat", "r");
  if(fin == NULL){
    printf("error: 'lfsr.dat' is not found\n");
    exit(1);
  }
  fscanf(fin, "%d", &max);
nfin=(int)nfin*atof(argv[3]);
printf("%d\n",nfin);
  if(nfin > max){
	loop=nfin/max+1;
	nfin_max=max;
    printf("warning: too many fins! loop=%d\n",loop);
   // fclose(fin);
   // exit(1);
  }
if(nfin<=max){
  for(ia=0; ia<nfin; ia++)
    if(fgets(buf, BUF_MAX, fin) == NULL){
      printf("error: too many fins!\n");
      fclose(fin);
      exit(1);
    }
  fscanf(fin, "%d", &XOR[0]);
  for(ia=1; ia<=XOR[0]; ia++)
    fscanf(fin, "%d", &XOR[ia]);
  fclose(fin);
	X=malloc((nfin+1)*sizeof(int));
}
else {
  for(ia=0; ia<nfin_max; ia++)
    if(fgets(buf, BUF_MAX, fin) == NULL){
      printf("error: too many fins!\n");
      fclose(fin);
      exit(1);
    }
  fscanf(fin, "%d", &XOR[0]);
  for(ia=1; ia<=XOR[0]; ia++)
    fscanf(fin, "%d", &XOR[ia]);
  fclose(fin);
  X_LOOP=malloc((nfin_max+1)*sizeof(int));

}



#if PHASE_SHIFT
  int Y[nfin+1];
#endif

 test_vec = (int)(atoi(argv[2])*atof(argv[4]));
fout = fopen("tmp_gt.dat", "w");
  fprintf(fout, "%d %d\n", test_vec, nfin);
  
if(nfin<=max){
  for(ia=0; ia<nfin; ia++){
if(ia%4==0)
    X[ia] = 1;
else     X[ia] = 0;
}
}
else{
  for(ia=0; ia<nfin_max; ia++)
if(ia%4==0)
    X_LOOP[ia] = 1;
else     X_LOOP[ia] = 0;

}

#if PHASE_SHIFT
  for(ia=0; ia<nfin; ia++)
    Y[ia] = X[ia];

  phase_shifter(Y, nfin);

  for(ia=0; ia<nfin; ia++)
    fprintf(fout, " %d", Y[ia]);
#else
/*if(nfin<=max){
  for(ia=0; ia<nfin; ia++)
    fprintf(fout, " %d", X[ia]);
}
else{
tmp_fin=0;
for(ib=0;ib<loop;ib++){
  for(ia=0; ia<nfin_max; ia++){
    fprintf(fout, " %d", X_LOOP[ia]);
	tmp_fin++;
	if(tmp_fin==nfin) break;
	}
}
}
*/
#endif
 // fprintf(fout, "\n");

  for(ia=1; ia<test_vec; ia++){
if(nfin<=max){
    for(ib=nfin; ib>=1; ib--)
      X[ib] = X[ib-1];

    for(ib=1; ib<XOR[0]; ib++)
      X[XOR[ib+1]] ^= X[nfin];
    X[0] = X[nfin];

#if PHASE_SHIFT
    for(ib=0; ib<nfin; ib++)
      Y[ib] = X[ib];

    phase_shifter(Y, nfin);

    for(ib=0; ib<nfin; ib++)
      fprintf(fout, " %d", Y[ib]);
#else
    for(ib=0; ib<nfin; ib++)
      fprintf(fout, " %d", X[ib]);
#endif
    fprintf(fout, "\n");
}

else{
tmp_fin=0;
for(ic=0;ic<loop;ic++){
    for(ib=nfin_max; ib>=1; ib--)
      X_LOOP[ib] = X_LOOP[ib-1];

    for(ib=1; ib<XOR[0]; ib++)
      X_LOOP[XOR[ib+1]] ^= X_LOOP[nfin_max];
    X_LOOP[0] = X_LOOP[nfin_max];

#if PHASE_SHIFT
    for(ib=0; ib<nfin_max; ib++)
      Y[ib] = X_LOOP[ib];

    phase_shifter(Y, nfin_max);

    for(ib=0; ib<nfin_max; ib++)
      fprintf(fout, " %d", Y[ib]);
#else
    for(ib=0; ib<nfin_max; ib++){
      fprintf(fout, " %d", X_LOOP[ib]);
	tmp_fin++;
	if(tmp_fin==nfin) break;
	}
	if(tmp_fin==nfin) break;
#endif
	}

	fprintf(fout, "\n");//break;

}

}
  fprintf(fout, "END\n");

  fclose(fout);
if(nfin<=max){
free(X);}
else{
free(X_LOOP);}
  return 0;
}

void phase_shifter(int X[], int nfin){
  int ia;
  for(ia=1; ia<nfin; ia++)
    X[ia] ^= X[ia-1];
}

void LT_RTPG(int X[],int RTPG[],int TFF[],int nfin){
	int ix,ik;
	for(ix=0;ix<nfin;ix++){//RTPG[ix]=X[ix];
		for(ik=ix+1;ik<ix+K;ik++){
			if(ik>=nfin) RTPG[ix]&=X[ik-nfin];
			else RTPG[ix]&=X[ik];
			}
		//if(RTPG[ix]==0) TFF[ix]=X[ix];
		if(RTPG[ix]==1) TFF[ix]=(~TFF[ix])&1;
	}

}

