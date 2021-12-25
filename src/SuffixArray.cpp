#include <iostream>
using namespace std;

struct element
{
	int ind, r1, r2;
};

class SuffixArray
{
	int size;
	element* array;
	const char* str;
    int* suffixArray;
    //helper functions:
    void mergeRank(int const left, int const mid, int const right);// used in mergeSortRank
    void mergeSortRank(int const begin, int const end);// recursive function to sort array using <r1,r2>.
    void mergeIndex(int const left, int const mid, int const right);// used in mergeSortIndex
    void mergeSortIndex(int const begin, int const end);// recursive function to sort array using ind.
public:
	SuffixArray(const char* s);
	void ConstructUsingPrefixDoubling();
	void Print();
};

int main()
{
    SuffixArray t("ACGACTACGATAAC$");
    t.ConstructUsingPrefixDoubling();
    t.Print(); // Prints:  14 11 12  0  6  3  9 13  1  7  4  2  8 10  5
    return 0;
}

void SuffixArray::mergeRank(int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto* leftArray = new element[subArrayOne],
        * rightArray = new element[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].r1 < rightArray[indexOfSubArrayTwo].r1 ||
            (leftArray[indexOfSubArrayOne].r1 == rightArray[indexOfSubArrayTwo].r1 &&
                leftArray[indexOfSubArrayOne].r2 <= rightArray[indexOfSubArrayTwo].r2)) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void SuffixArray::mergeSortRank(int const begin, int const end)
{
    if (begin >= end)
        return; //Base case

    auto mid = begin + (end - begin) / 2;
    mergeSortRank(begin, mid);
    mergeSortRank(mid + 1, end);
    mergeRank(begin, mid, end);
}

void SuffixArray::mergeIndex(int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto* leftArray = new element[subArrayOne],
        * rightArray = new element[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].ind <= rightArray[indexOfSubArrayTwo].ind) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void SuffixArray::mergeSortIndex(int const begin, int const end)
{
    if (begin >= end)
        return; //Base case

    auto mid = begin + (end - begin) / 2;
    mergeSortIndex(begin, mid);
    mergeSortIndex(mid + 1, end);
    mergeIndex(begin, mid, end);
}

SuffixArray::SuffixArray(const char* s)
{
	str = s;
	size = 0;
	while (str[size] != '\0')
	{
		size++;
	}
	array = new element[size];
	for (int i = 0; i < size; i++)
	{
		array[i].ind = i;
	}
    suffixArray = new int[size];
}

void SuffixArray::ConstructUsingPrefixDoubling()
{
    //set r1 and r2 of the first iteration to sort according to the first 2 characters in each suffix.
    for (int i = 0; i < size - 1; i++)
    {
        array[i].r1 = str[i] - 'A' + 1; //+1 because 0 is assigned to '$'
        array[i].r2 = str[i + 1] - 'A' + 1; //+1 because 0 is assigned to '$'
    }

    //'$' rank
    array[size - 1].r1 = 0;
    array[size - 1].r2 = -1;

    for (int k = 2; k <= size; k *= 2)
    {
        //sort using <r1,r2>, according to the first k characters in each suffix.
        mergeSortRank(0, size - 1);

        //Move sequentially on the array elements to update r1 for each element.
        int rank = 0, previousRank, temp;
        for (int i = 0; i < size; i++)
        {
            temp = array[i].r1;
            array[i].r1 = ((i - 1) >= 0) ? ((array[i].r1 == previousRank && array[i].r2 == array[i - 1].r2) ? rank : ++rank) : 0;
            previousRank = temp;
        }

        //Sort the above array using ind.
        mergeSortIndex(0, size - 1);

        //Move sequentially on the array elements to update r2 for each element.
        for (int i = 0; i < size; i++)
        {
            array[i].r2 = (i + k < size) ? array[i + k].r1 : -1;
        }
    }
    // Store indexes of all sorted suffixes in the suffix array
    for (int i = 0; i < size; i++)
    {
        suffixArray[array[i].r1] = array[i].ind;
    }
}

void SuffixArray::Print()
{
	for (int i = 0; i < size; i++)
	{
		cout << suffixArray[i] << ' ';
	}
}