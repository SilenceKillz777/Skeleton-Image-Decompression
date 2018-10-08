//Skeleton Image Decompression
#include <iostream>
#include <fstream>
using namespace std;

class imageProcessing{
	public:
		
	int numRows, numCols, minVal, maxVal;
	int** deCompressedAry;
	int neighborAry[5];
	
	//constructor
	imageProcessing(int numRows, int numCols, int minVal, int maxVal){
		
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
			
		deCompressedAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++)
			deCompressedAry[i] = new int[numCols+2];
	}
	
	//methods
	void initialize(){	
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				deCompressedAry[i][j] = 0;
			}
		}
	}
	
	void loadImage(string input,ofstream& outFile2){
		int r, c, d;
		ifstream in;
		in.open(input.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;
		while(!in.eof()){
			in>>r>>c>>d;
			deCompressedAry[r+1][c+1] = d;
		}
	}
	
	void loadNeighbors(int rows, int cols) {
		neighborAry[0] = deCompressedAry[rows][cols];
		neighborAry[1] = deCompressedAry[rows-1][cols];
		neighborAry[2] = deCompressedAry[rows][cols-1];
		neighborAry[3] = deCompressedAry[rows][cols+1];
		neighborAry[4] = deCompressedAry[rows+1][cols];
	}
	
	int max(int pass){
		int max = 0;
		if(pass==1){
			max = neighborAry[1];
			if(neighborAry[2]>max)
				max = neighborAry[2];
		}
		else if(pass==2) {
			max = neighborAry[3];
			if(neighborAry[4]>max){
				max = neighborAry[4];
			}
		}
		return max;
	}
	
	void firstPass_deCompress(){
		for(int rows=1;rows<numRows+1;rows++) {
			for(int cols=1;cols<numCols+2;cols++) {
				if(deCompressedAry[rows][cols] == 0){
					loadNeighbors(rows,cols);
					if(max(1)>0){
						deCompressedAry[rows][cols] = max(1)-1;
					}
				}
			}
		}
	}
	
	void secondPass_deCompress(){
		for(int rows=numRows;rows>0;rows--) {
			for(int cols=numCols;cols>0;cols--) {
			loadNeighbors(rows,cols);
				if(deCompressedAry[rows][cols] < neighborAry[3]||deCompressedAry[rows][cols] < neighborAry[4]){
					if(max(2)>0){
						deCompressedAry[rows][cols] = max(2)-1;
					}
				}
			}
		}
	}
	
	void outputDecompressImg(ofstream& outFile1){
		for(int rows=1;rows<numRows+1;rows++){
			for(int cols=1;cols<numCols+1;cols++)
				outFile1<<deCompressedAry[rows+1][cols+1]<<" ";
			outFile1<<endl;
		}
	}
	
	void prettyPrint(int pass, ofstream& outFile2){
		outFile2<<"The result of pass-"<<pass<<" decompression"<<endl;
		for(int rows=1;rows<numRows+1;rows++){
			for(int cols=1;cols<numCols+1;cols++){
				if(deCompressedAry[rows+1][cols+1]!=0){
					outFile2<<deCompressedAry[rows+1][cols+1]<<" ";
				}
				else{
					outFile2<<"  ";
				}
			}
			outFile2<<endl;
		}
		outFile2<<endl;
	}
};

int main(int argc, char* argv[]){
	ifstream input (argv[1]);
	string fileName = argv[1];
	ofstream outFile1 (argv[2]);
	ofstream outFile2 (argv[3]);
	int numRows, numCols, minVal, maxVal;
	
	if(input.is_open()){
		input>>numRows>>numCols>>minVal>>maxVal;
		outFile1<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		imageProcessing process(numRows, numCols, minVal, maxVal);
		process.initialize();
		process.loadImage(fileName,outFile2);
		process.firstPass_deCompress();
		process.prettyPrint(1,outFile2);
		process.secondPass_deCompress();
		process.prettyPrint(2,outFile2);
		process.outputDecompressImg(outFile1);
		input.close();
		outFile1.close();
		outFile2.close();
	}
	else cout<<"Couldn't retrieve data.";
	return 0;
}
