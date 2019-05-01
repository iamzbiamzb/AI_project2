#include "input_handle.h"
#include <map>
#include <set>
#include <time.h>
#include <stdlib.h>

int Maxdepth ;

struct Data{
	vector<float> Attribute ;
	int target ;
};

struct ThresholdReturn{
	int AttributeNum ;
	float Threshold ;
	float Gini_impurity ;
};

int cmpint ;

bool cmp(const Data &a , Data &b){
    return a.Attribute[cmpint] < b.Attribute[cmpint] ;
}

bool cmpThreshold(const ThresholdReturn &a , ThresholdReturn &b){
    return a.Gini_impurity < b.Gini_impurity ;
}

struct Node{
	Node *left;
	Node *right;
	Node *parent;
	float Threshold;
	int AttributeNum ;
	int depth ;
	int target ;
};

void Tansfer( vector<vector<string>> Origindataset, vector<Data>& Datasets ){
	map< string , int > MapTarget ;
	int TargetNumber = 0 ;
	for ( int i = 0 ; i < Origindataset.size() ; i++ ){
		Data tmpData ;
		for ( int j = 0 ; j < Origindataset[i].size() ; j++ ){
			if ( j == Origindataset[i].size()-1 ){
				if ( MapTarget.find(Origindataset[i][j]) == MapTarget.end() ){
					MapTarget[Origindataset[i][j]] = TargetNumber ;
					tmpData.target = TargetNumber ;
					TargetNumber++ ;
				}else{
					tmpData.target = MapTarget.find(Origindataset[i][j])->second;
				}
			}else {
				tmpData.Attribute.push_back(stof(Origindataset[i][j])); 
			}
		}
		Datasets.push_back(tmpData);
	}
}

void split( vector<Data> dataset, vector<Data>& Train, vector<Data>& Test, int percenttage ){
	srand( time(NULL) );
	int TestCnt = dataset.size()*percenttage/100;
	while( TestCnt != 0 ){
		int tmp = rand()%dataset.size();
		Test.push_back(dataset[tmp]);
		dataset.erase(dataset.begin()+tmp);
		TestCnt--;	
	}
	for( int i = 0 ; i < dataset.size() ; i++ )
		Train.push_back(dataset[i]);
}

float Gini(vector<Data> a , vector<Data> b){
	float Ga = 1 , Gb = 1 ;
	int GacntMax = 0 , GbcntMax = 0 ;
	int Gacnt[2000] = {0};
	int Gbcnt[2000] = {0}; 
	for ( int i = 0 ; i < a.size() ; i++ ){
		Gacnt[a[i].target]++;
		if ( a[i].target > GacntMax ) GacntMax = a[i].target ;
	}
	for ( int i = 0 ; i < b.size() ; i++ ){
		Gbcnt[b[i].target]++;
		if ( b[i].target > GbcntMax ) GbcntMax = b[i].target ;
	}
	for ( int i = 0 ; i < GacntMax+1 ; i++ ){
		Ga -= (float)(Gacnt[i]*Gacnt[i])/(float)(a.size()*a.size());
	}
	for ( int i = 0 ; i < GbcntMax+1 ; i++ ){
		Gb -= (float)(Gbcnt[i]*Gbcnt[i])/(float)(b.size()*b.size());
	}
	return (float)a.size()/(float)(a.size()+b.size())*Ga+(float)b.size()/(float)(a.size()+b.size())*Gb;
}

ThresholdReturn ThresholdFind(int Attrubuteint, vector<Data> Datasets ){

	ThresholdReturn tmpThresholdReturn;
	tmpThresholdReturn.Gini_impurity = 1 ;
	tmpThresholdReturn.AttributeNum = Attrubuteint ;
	cmpint = Attrubuteint ;

	sort(Datasets.begin(),Datasets.end(),cmp);
	for ( int i = 0 ; i < Datasets.size()+1 ; i++ ){
		vector<Data> a ;
		vector<Data> b ;

		for ( int j = 0 ; j < Datasets.size() ; j++ ){
			if ( j < i )
				a.push_back(Datasets[j]);
			else
				b.push_back(Datasets[j]);
		}
		float tmpGini = Gini(a,b);
		if ( tmpGini < tmpThresholdReturn.Gini_impurity ){
			tmpThresholdReturn.Threshold = (a[a.size()-1].Attribute[cmpint]+b[0].Attribute[cmpint])/2 ;
			tmpThresholdReturn.Gini_impurity = tmpGini ;
		}
	}

	return tmpThresholdReturn;
}

void MakeTree( vector<Data> Datasets , Node* parent , set<int> setAttribute ){

	vector<ThresholdReturn> tmpThresholdReturnSet ;
	for ( int i = 0 ; i < Datasets[0].Attribute.size() ; i++ ){
		if ( setAttribute.find(i) != setAttribute.end() )continue;
		tmpThresholdReturnSet.push_back(ThresholdFind(i,Datasets)); 
	}

	sort(tmpThresholdReturnSet.begin(),tmpThresholdReturnSet.end(),cmpThreshold);

	vector<Data> a ;
	vector<Data> b ;

	for ( int i = 0 ; i < Datasets.size() ; i++ ){
		if ( Datasets[i].Attribute[tmpThresholdReturnSet[0].AttributeNum] < tmpThresholdReturnSet[0].Threshold ){
			a.push_back(Datasets[i]);
		}else{
			b.push_back(Datasets[i]);
		} 

	}

	parent->AttributeNum = tmpThresholdReturnSet[0].AttributeNum ;
	parent->Threshold = tmpThresholdReturnSet[0].Threshold ;
	setAttribute.insert(tmpThresholdReturnSet[0].AttributeNum);

	Node* leftchild = new Node ;
	Node* rightchild = new Node ;
	leftchild->parent = parent ;
	rightchild->parent = parent ;
	leftchild->target = -1 ;
	rightchild->target = -1 ;
	leftchild->depth = parent->depth + 1 ;
	rightchild->depth = parent->depth + 1 ;
	parent->left = leftchild ;
	parent->right = rightchild ;

	if ( parent->depth > Maxdepth || a.size() == 0 || b.size() == 0 || setAttribute.size() == Datasets[0].Attribute.size() ){

		int secondTarget ;
		if ( a.size() != 0 ){
			int Target[2000] = {0} ;
			int max = 0 , maxnum = 0 ;
			for ( int i = 0 ; i < a.size() ; i++ )
				Target[a[i].target]++;
			for ( int i = 0 ; i < 2000 ; i++ ){
				if( Target[i] > max ){
					max = Target[i] ;
					maxnum = i ;
				}
			}
			leftchild->target =  maxnum;
			Target[maxnum] = 0 ;
			for ( int i = 0 ; i < 2000 ; i++ ){
				if( Target[i] > max ){
					max = Target[i] ;
					maxnum = i ;
				}
			}
			secondTarget =  maxnum;
			
		}
		if ( b.size() != 0 ){
			int Target[2000] = {0} ;
			int max = 0 , maxnum = 0 ;
			for ( int i = 0 ; i < b.size() ; i++ )
				Target[b[i].target]++;
			for ( int i = 0 ; i < 2000 ; i++ ){
				if( Target[i] > max ){
					max = Target[i] ;
					maxnum = i ;
				}
			}
			rightchild->target =  maxnum;
			Target[maxnum] = 0 ;
			for ( int i = 0 ; i < 2000 ; i++ ){
				if( Target[i] > max ){
					max = Target[i] ;
					maxnum = i ;
				}
			}
			secondTarget =  maxnum;
		}
		
		if ( a.size() == 0 )leftchild->target = secondTarget ;
		if ( b.size() == 0 )rightchild->target = secondTarget ;	
		leftchild->right = NULL;
		leftchild->left = NULL;
		rightchild->right = NULL;
		rightchild->left = NULL;

	}else{
		MakeTree(a,leftchild,setAttribute);
		MakeTree(b,rightchild,setAttribute);
	}	
}

void PrintTree(Node* root){
	if ( root->left != NULL || root->left != NULL ){
		cout << "treeNode:" << endl ;
		cout << "Threshold:" << root->Threshold << endl ;
		cout << "AttributeNum:" << root->AttributeNum << endl ;
		cout << "depth:" << root->depth << endl;
		cout << endl ; 
		PrintTree(root->left);
		PrintTree(root->right);
	}else{
		cout << "LeafNode:" << endl ;
		cout << "Target:" << root->target << endl ;
		cout << endl ; 
	}
}

void Predict(vector<Data> Test , vector<int> &ans , Node* root ){
	for ( int i = 0 ; i < Test.size() ; i++ ){
		Node* ptr = root ;
		while(1){
			if( ptr->left == NULL && ptr->right == NULL ){
				ans.push_back(ptr->target);				
				break;
			}else{
				if ( Test[i].Attribute[ptr->AttributeNum] < ptr->Threshold ) ptr = ptr->left ;
				else ptr = ptr->right ;
			}
		}
	}
}

float Score( vector<Data> Test , vector<int> ans ){
	float correct = 0 ;
	for( int i = 0 ; i < Test.size() ; i++ ){
		if( Test[i].target == ans[i] )
			correct = correct + 1 ;
	}
	return correct/Test.size();
}

void opening ( int &percenttage , int & treenum , string & name , string & del ){
	int namenum ;
	cout << "RandomForset tree: ";
	cin >> treenum ;
	cout << "test data percenttage: ";
	cin >> percenttage ;
	cout << "choose data: " << endl ;

	cout << "iris.data      = 0 \n";
	cout << "ellipse100.txt = 1 \n";
	cout << "cross200.txt   = 2 \n";
	cin >> namenum ;
	switch(namenum){
		case 0:
			name = "iris.data";
			break;
		case 1:
			name = "ellipse100.txt";
			break;
		case 2:
			name = "cross200.txt";
			break;
		case 4:
			name = "iris.data";
			break;
	}
	cout << "depth: ";
	cin >> Maxdepth ;
	del = " ,";

}

int main (){

	/////////////
	///declare///
	/////////////
	int percenttage, treenum; 
	float treescore = 0 ;
	string name;
	string del ;
	vector<vector<string>> Origindataset ;
	vector<Data> Datasets ;
	vector<Data> Train ;
	vector<Data> Test ;
	root->depth = 0 ;
	set<int> setAttribute ;
	vector<int> ans ; 
	//////////////
	///function///
	//////////////

	opening(percenttage,treenum,name,del);
	InputCSV( name, del, Origindataset );
	//InputCSVtest(Origindataset);
	Tansfer(Origindataset,Datasets);
	//split(Datasets,Train,Test,percenttage);
	//MakeTree(Train,root,setAttribute);
	//PrintTree(root);
	//Predict(Test,ans,root);
	//cout << Score(Test,ans) << endl ;

	for ( int i = 0 ; i < treenum ; i++ ){
		Node* root = new Node ;
		split(Datasets,Train,Test,max(10,rand()%30));
		MakeTree(Train,root,setAttribute);
		Predict(Test,ans,root);
		treescore = max(treescore,Score(Test,ans));
		Train.clear();
		Test.clear();
	}
	cout << "treescore: " << treescore << endl ;

}