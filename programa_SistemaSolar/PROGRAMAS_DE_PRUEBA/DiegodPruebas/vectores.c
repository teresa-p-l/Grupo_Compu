
#include <stdio.h>

//We now define the max lines and max length of the lines to not saturate the pc

#define MAX_LINES 100
#define MAX_LEN 1000

//Now we define the function that will read the lines and store them in a vector

int main(void)
{
    char data[MAX_LINES][MAX_LEN]; //We define the vector that will store the lines

    FILE *file; //We define the file pointer

    file = fopen("chat.txt", "r"); //We open the file in read mode

    //We can check if something has gone wrong with the file opening
    if (file == NULL) {
        printf("Error opening file\n");
        return 1; //Returning 1 is a signal to the terminal that something has gone wrong
    }

    //We can now read the lines of the file and store them in the vector
    
    int line=0; //We define the line counter

    //To read the data we will have a loop that ends if there is an error or if it ends.

    while(!feof(file) && !ferror(file)) 
    //So long as we havent reached the end of the file, continue.
    //And so long as there is no error, continue.
    {
        if (fgets(data[line], MAX_LEN, file) != NULL) //We read the line from 'file' and store it in the vector data at position 'line' and IT HASNT ENDED.
            line++; //We increment the line counter
    }

    //We are now ready to close the file

    fclose(file); //We close it.

    //We can check the number of data lines we have read.
    for (int i=0; i<line; i++)
    {
        printf("%s", data[i]); //We print the lines we have read.
    }

    return 0; //We return 0 to signal that everything has gone well.

}









//Now lets make it a function with FLOAT numbers

// Function to read lines from a file and store them in a vector
int read_file_to_vector(const char *filename, float data[MAX_LINES][MAX_LEN]) {
    FILE *file = fopen(filename, "r"); // Open the file in read mode

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 1; // Return 1 to indicate an error
    }

    int line = 0; // Line counter

    // Read the file line by line
    while (!feof(file) && !ferror(file) && line < MAX_LINES && fscanf(file, "%f", &data[line]) == 1) {
        line++; // Increment the line counterç
        
        //The %f is float numbers
    }

    fclose(file); // Close the file
    return 0; // Return 0 to indicate success
}