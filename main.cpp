#include <iostream>
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
using namespace std;

void convert_vector_to_string(vector<string> &input_arr,vector<int> &int_input_arr){
    for(int i =0; i<int_input_arr.size();i++){
        input_arr.push_back(to_string(int_input_arr.at(i)));
    }
}
void convert_vector_to_int(vector<string> &decompressed_arr,vector<int> &int_decompressed_array){
    for(int i =0; i<decompressed_arr.size();i++){
        string code= decompressed_arr.at(i);
        int no=0;
        for(int j=0;j<code.size();j++){
            no = no * 10 + (code[j] - '0');
        }
        int_decompressed_array.push_back(no);
    }
}
int present(string find, vector<string> &dictionary_entry)
{
    for (int i = 0; i < dictionary_entry.size(); i++)
    {
        if (dictionary_entry.at(i) == find)
        {
            return 1;
        }
    }
    return 0;
}
int find_index_code(string code_string, vector<string> &dictionary_entry)
{
    for (int i = 0; i < dictionary_entry.size(); i++)
    {
        if (code_string == dictionary_entry.at(i))
        {
            return i;
        }
    }
    return -1;
}

void image_compression(vector<string> &input_arr, vector<string> &dictionary_entry, vector<int> &encoded_output)
{
    // INTIALIZING DICTIONARY ENTRY
    for (int i = 0; i < 256; i++)
    {

        dictionary_entry.push_back(to_string(i));
    }

    cout << "COMPRESSION STARTED...\n"
         << "PLEASE WAIT A MOMENT WHILE YOUR IMAGE GETS COMPRESSED\n";
    // string -> pixel_being_processed -> currently read symbol from input_arr
    // string -> code string -> code string will concat  with pixel_being_processed if not present   in dict_entry then add the code string in dict_entry array and set the code_string to pixel_being_processed else concat code_string and pixel_being_processed.
    string code_string = "";
    string pixel_being_processed = "";
    int encoded_counter = 0;
    for (int i = 0; i < input_arr.size(); i++)
    {
        // cout<<code_string<<"\t"<<pixel_being_processed<<"\t";
        pixel_being_processed = input_arr[i];
        // cout << code_string << " ";
        if (!present(code_string + "," + pixel_being_processed, dictionary_entry) && code_string != "")
        {
            // cout<<find_index_code(code_string, dictionary_entry)<<"\t"<<code_string + "," + pixel_being_processed;
            dictionary_entry.push_back(code_string + "," + pixel_being_processed);
            encoded_output.push_back(find_index_code(code_string, dictionary_entry));
            code_string = pixel_being_processed;
            encoded_counter++;
            if (i == input_arr.size() - 1)
            {
                // cout<<"\t\t"<<find_index_code(code_string, dictionary_entry);
                encoded_counter++;
                encoded_output.push_back(find_index_code(code_string, dictionary_entry));
            }
        }

        else
        {
            if (code_string == "")
            {
                code_string = pixel_being_processed;
            }

            else
            {
                code_string = code_string + "," + pixel_being_processed;
                if (i == input_arr.size() - 1)
                {
                    // cout<<"\t\t"<<find_index_code(code_string, dictionary_entry);
                    encoded_counter++;
                    encoded_output.push_back(find_index_code(code_string, dictionary_entry));
                }
            }
        }
    }
    cout << "COMPRESSION FINISHED\n";
    cout << "dictionary entry size " << dictionary_entry.size() << "\n";
    cout << "encoded output size " << encoded_output.size() << "\n";
    cout << "TOTAL BYTES SAVED = " << ((8 * input_arr.size()) - (16 * encoded_counter)) / 8 << "\n";
}
void image_decompression(vector<string> &dictionary_entry, vector<int> &encoded_output, vector<string> &decompressed_array)
{
    cout << "DECOMPRESSION STARTED\n";
    for (int i = 0; i < encoded_output.size(); i++)
    {
        string temp = dictionary_entry.at(encoded_output.at(i));
        string s1 = "";
        int comma = 0;
        for (int j = 0; j < temp.length(); j++)
        {
            if (temp[j] != ',')
            {
                s1 = s1 + temp[j];
                comma = 0;
            }
            else
            {
                comma = 1;
                decompressed_array.push_back(s1);
                s1 = "";
            }
        }
        if (comma == 0)
        {
            decompressed_array.push_back(s1);
        }
    }
    cout << "OPENING IMAGE IN OTHER WINDOW...\n";
}
int main(int, char **)
{
    Mat image;
    image = imread("D:/M.Tech/Semester 1/CS524/project/img_236x180.jpg",IMREAD_GRAYSCALE);
    if (!image.data)
    {
        cout << "no Image data\n";
        return -1;
    }
    vector<string> input_arr;
    vector<int> int_input_arr;
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            int p = image.at<uchar>(i, j);
            int_input_arr.push_back(p);
        }
    }
    vector<string> dictionary_entry;
    vector<int> encoded_output;
    vector<string> decompressed_array;
    vector<int> int_decompressed_array;
    
    convert_vector_to_string(input_arr,int_input_arr);
    image_compression(input_arr, dictionary_entry, encoded_output);
    image_decompression(dictionary_entry,encoded_output,decompressed_array);
    convert_vector_to_int(decompressed_array,int_decompressed_array);
     if(int_input_arr==int_decompressed_array){
        cout<<"COMPRESSION AND DECOMPRSSION WORKS PERFECT\n";
    }
    else{
        cout<<"COMPRESSION AND DECOMPRSSION does not WORKS PERFECT\n";
    }

    Mat img(image.rows, image.cols, CV_8UC1);
     int counter = 0;
    for(int i = 0;i<image.rows;i++){
        for(int j=0; j<image.cols;j++){
            img.at<uchar>(i,j)=int_decompressed_array.at(counter);
            counter++;
        }
    }
    img = imwrite("D:/M.Tech/Semester 1/CS524/project/output_img_236x180.jpg", img);
   
    // namedWindow("Display Image", WINDOW_AUTOSIZE);
    // imshow("Display Image", img);
    waitKey(0);
    return 0;
}
