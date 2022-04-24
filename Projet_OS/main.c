#include <stdio.h>
#include <stdlib.h>

struct process{
    int id;
    int DA;//Date d'arrive
    int DE;//Dure exe
    int TF;// temps fin
    int SE;//debut execution
    int TS;// temps sejour
    int wait;// temps d'attente
    int TR;
};

typedef struct cellProcess cellProcess;

struct cellProcess {
    struct process proc;
    struct cellProcess *next;

};

typedef cellProcess *listProcess;

struct process createProcess(){
    struct process nouvProc;
    printf("Id:");
    scanf("%d",&nouvProc.id);
    printf("Date d'arrive:");
    scanf("%d",&nouvProc.DA);
    printf("Duree d'execution");
    scanf("%d",&nouvProc.DE);
    return nouvProc;
};



listProcess ajouterEnTete(listProcess head){
    cellProcess* nouv=malloc(sizeof(cellProcess));
    nouv->proc=createProcess();
    nouv->next=NULL;
    head=nouv;
    return head;
}


listProcess addProcess_inOrder(listProcess head){
    struct process med;
    listProcess cour, prec;
    int i;
    cellProcess* nouv=malloc(sizeof(cellProcess));
    nouv->proc=createProcess();
        cour=head;
        //printf("\nCour %x = head=%x",cour,head);

        if(cour->proc.DA >= nouv->proc.DA){
                if( (cour->proc.DA == nouv->proc.DA) && (nouv->proc.DE < cour->proc.DE)){
                        med=cour->proc;
                        cour->proc=nouv->proc;
                        nouv->proc=med;
                        nouv->next=cour->next;
                        cour->next=nouv;
                }
                else{
                    cour->next=cour;
                    head=nouv;
                }
                //printf("\n cas superier");
                return head;
            }
        else{
                //printf("\n cas inferieur");
                prec=cour;
                //printf("\nPrec %x",prec);
                cour=cour->next;
                //printf("\nCour %x",cour);
                i=0;
                while(cour!=NULL && cour->proc.DA < nouv->proc.DA ){
                    prec=cour;
                    //printf("\nPrec %x",prec);
                    cour=cour->next;
                    //printf("\nCour %x",cour);
                    //printf("\n i=%d",i);
                    i++;
                }
                //printf("%x",cour);
                //printf("prec->next=nouv;");
                if((cour!=NULL)){

                if( (cour->proc.DA == nouv->proc.DA) && (nouv->proc.DE > cour->proc.DE)){
                        prec=cour;
                        cour=cour->next;

                }}

                    prec->next=nouv;
                    nouv->next=cour;

                //printf("nouv->next=cour;");
                return head;
        }
}


listProcess add_process_fcfs_and_order(listProcess head){
    //Cas d'une liste vide, ajout au debut commune aux deux systeme d'ordonancement
    if(head==NULL){
        return ajouterEnTete(head);
    }
    // Sinon ajouter selon le type d'ordonnancement c'est en ordre de  duree ou de Temps d'arrive
    else{
       return addProcess_inOrder(head);
    }
}

listProcess add_process_sjf(listProcess head){

    //Cas d'une liste vide, ajout au debut commune aux deux systeme d'ordonancement
    if(head==NULL){
        return ajouterEnTete(head);
    }
    // Sinon ajouter selon le type d'ordonnancement c'est en ordre de  duree ou de Temps d'arrive
    else{
       return addProcess_inOrder(head);}


}

struct process getProcess(listProcess head){
    return head->proc;
}

listProcess recherElement(listProcess head , int min){
    listProcess cour;
    cour=head;
    while (cour!=NULL)
    {
        if(cour->proc.DE==min){
            return cour;
        }
        cour=cour->next;
    }

return cour;
}


listProcess findMin(listProcess head){
    int min;
    listProcess cour;
    cour=head;
    min=head->proc.DE;
    while (cour!=NULL)
    {
        if(cour->proc.DE<min){
            min=cour->proc.DE;
        }
        cour=cour->next;
    }
   return recherElement(head,min);

}

listProcess final_SJF(listProcess head,int nb_process){

    listProcess cour,p,b,courA,courF,precA;
    int i,TF,a;
    listProcess listAtt=NULL;
    listProcess listFinal=NULL;
    listProcess listE=head;

    //detacher le premier element et mettre dans liste final
    listFinal=listE;
    listE=listE->next;
    listFinal->next=NULL;
    listFinal->proc.SE=listFinal->proc.DA;
    listFinal->proc.TF=listFinal->proc.DA+listFinal->proc.DE;
    TF=listFinal->proc.TF;

    for(i=1;i<nb_process;i++){
        a=0;
        p=listE;
        //Remplire la liste attente
        while (p!=NULL && p->proc.DA<=TF)
        {
            if(listAtt==NULL){// Liste attente vide
                listAtt=p;
                listE=p->next;
                p=listE;
                listAtt->next=NULL;
            }
            else{ //Liste attente non vide

                cour=listAtt;
                while(cour->next!=NULL)
                {
                    a++;
                    cour=cour->next;
                }
                cour->next=p;
                listE=p->next;
                p->next=NULL;
                p=listE;
            }
        }
        /*Remplissage de la liste final */
        if(listAtt==NULL){
            //Extraire le premier de la liste ordonne par date si pas de liste d'attente
                cour=listFinal;
                while (cour->next!=NULL)
                {
                    cour=cour->next;
                }
                cour->next=p;
                // SE= debut execution
                cour->next->proc.SE=TF;
                cour->next->proc.TF=TF+cour->next->proc.TF;
                TF=cour->next->proc.TF;
                listE=p->next;
                p->next=NULL;


        }
        else{
        //Extraire le plus petit de la liste d'attente etla brancher à final

            if (findMin(listAtt)==listAtt)//l'element est en premiere position
            {
               cour=listFinal;
               b=findMin(listAtt);
               while (cour->next!=NULL)
                {
                    cour=cour->next;
                }
                cour->next=b;
                cour->next->proc.SE=TF;
                cour->next->proc.TF=TF+cour->next->proc.TF;
                TF=cour->next->proc.TF;
                listAtt=listAtt->next;
                b->next=NULL;

            }
            else{ // Min n'est pas en tete de la liste
                courA=listAtt;
                courF=listFinal;
                while (courA!=findMin(listAtt))
                {
                    precA=courA;
                    courA=courA->next;
                }
                while (courF->next!=NULL)
                {
                    courF=courF->next;
                }
                courF->next=courA;
                courF->next->proc.SE=TF;
                courF->next->proc.TF=TF+courF->next->proc.TF;
                TF=courF->next->proc.TF;
                precA->next=courA->next;
                courA->next=NULL;
            }
        }
    }


return listFinal;

}


listProcess Calcul_Details(listProcess head){
    listProcess p,prec;
    p=head;
    p->proc.SE=p->proc.DA;
    p->proc.TF=p->proc.SE+p->proc.DE;
    p->proc.wait=p->proc.TF-p->proc.DE-p->proc.DA;
    p->proc.TS=p->proc.TF-p->proc.DA;
    prec=p;
    p=p->next;
    while(p!=NULL){
        if(prec->proc.TF < p->proc.DA){
            p->proc.SE=p->proc.DA;
            p->proc.TF=p->proc.SE+p->proc.DE;
            p->proc.wait=p->proc.TF-p->proc.DE-p->proc.DA;
            p->proc.TS=p->proc.TF-p->proc.DA;
        }
        else{
            p->proc.SE=prec->proc.TF;
            p->proc.TF=p->proc.SE+p->proc.DE;
            p->proc.wait=p->proc.TF-p->proc.DE-p->proc.DA;
            p->proc.TS=p->proc.TF-p->proc.DA;
            }
        prec=p;
        p=p->next;
    }
    return head;
}



void afficheTest(listProcess head){
    listProcess p;
    p=head;
    while(p!=NULL){
        printf("\n |debut= %d",p->proc.SE);
        printf("\n |id= %d",p->proc.id);
        printf("\n |fin  = %d",p->proc.TF);
        printf("\n");
        p=p->next;
    }
}


void afficherliste(listProcess head)
{
   listProcess p;
   p=head;
while(p!= NULL)
{
    printf("------- %d\n",p->proc.SE);
    printf("|     |\n"); //printf("\t Temps de sejour= %d",p->proc.TS);
    printf("|  P%d |\n",p->proc.id);
    printf("|     |\n"); //printf("\t Temps d'attente= %d",p->proc.wait);
    printf("------- %d\n",p->proc.TF);
    p= p->next;
}
}

void afficheResult(listProcess head){
    listProcess p;
    int i;
    float wait_tot, ts_tot;
    p=head;
    i=0;
    wait_tot=0;
    ts_tot=0;
    printf("------------------------------------------\n");
    printf("|No process   |Temps attente|Temps sejour|\n");
    printf("|             |             |            |\n");
while(p!= NULL)
{
    printf("------------------------------------------\n");
    printf("|             |            |              |\n"); //printf("\t Temps de sejour= %d",p->proc.TS);
    printf("|   P%d       |  %d        |  %d          |\n",p->proc.id,p->proc.wait,p->proc.TS);
    printf("|             |            |              |\n"); //printf("\t Temps d'attente= %d",p->proc.wait);

    wait_tot=wait_tot+p->proc.wait;
    ts_tot=ts_tot+p->proc.TS;
    p= p->next;
    i++;
}
    printf("------------------------------------------\n");
    printf("|     Moyen   |   %.2f     |  %.2f      |\n",wait_tot/i,ts_tot/i);
    printf("|             |            |              |\n");
    printf("------------------------------------------\n");
}


void fcfs(){
        int nb_proc,i;
        listProcess listP;
        listP=NULL;
            printf("Saisir le nombre de processus");
            scanf("%d",&nb_proc);
            printf("\n\n");
            printf("Remplissage des processus");
            printf("\n");
            printf("\n");
            for(i=0;i<nb_proc;i++){
            listP=add_process_fcfs_and_order(listP);
            printf("\n");
            }
            //afficheTest(listP);
            listP=Calcul_Details(listP);
            afficherliste(listP);
            printf("Informations annexes");
            afficheResult(listP);
}


void sjf(){
        int nb_proc,i;
        listProcess listP;
        listP=NULL;
            printf("Saisir le nombre de processus");
            scanf("%d",&nb_proc);
            printf("\n\n");
            printf("Remplissage des processus");
            printf("\n");
            printf("\n");
            for(i=0;i<nb_proc;i++){
            listP=add_process_sjf(listP);
            printf("\n\n");}
            //afficheTest(listP);
            listP=final_SJF(listP,nb_proc);
            listP=Calcul_Details(listP);
            afficherliste(listP);
            printf("\nInformations annexes\n");
            afficheResult(listP);
            printf("\n");

}



int main()
{
    /*Declaration des variables*/
    char choix;
    system("COLOR E4");
    printf("\t\t\t*************************************************\n\t\t\t*\t\t\t\t\t\t*\n\t\t\t*\tWELCOME TO SCHEDULING SIMULATOR\t\t*\n\t\t\t*\t\t\t\t\t\t*\n\t\t\t*************************************************\n\n");
    //Le menu
    printf("Choisir le systeme d'ordonnancement \n");
    printf("\t1-FCS \n \t2-SJS");
    do {
        printf("\nVotre choix[1 ou 2]:");
        fflush(stdin);
        scanf("%c",&choix);
    }while(choix!='1' && choix!='2');

    switch(choix){

        case '1':
            fcfs();
            break;


        case '2':
            sjf();
            break;
    }

}
