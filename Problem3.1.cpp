#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  vector<int> L(n1), R(n2);

  for (int i = 0; i < n1; i++) L[i] = arr[left + i];
  for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}


void MergeSort(vector<int>& arr, int left, int right) {
  if (left >= right) return;

  int mid = left + (right - left) / 2;
  MergeSort(arr, left, mid);
  MergeSort(arr, mid + 1, right);
  merge(arr, left, mid, right);
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int N;
  vector<int> full_array;
  if (rank == 0) {
    cout << "Enter total number of elements (N): ";
    cin >> N;

    full_array.resize(N);
    cout << "Enter " << N << " integers separated by spaces: " << endl;
    for (int i = 0; i < N; i++) {
      cin >> full_array[i];
    }
  }

  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int chunk_size = N / size;
  vector<int> local_chunk(chunk_size);

  MPI_Scatter(full_array.data(), chunk_size, MPI_INT, local_chunk.data(),
              chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

  MergeSort(local_chunk, 0, chunk_size - 1);

  int step = 1;
  while (step < size) {
    if (rank % (2 * step) == 0) {
      if (rank + step < size) {
        int recv_size = chunk_size * step;
        vector<int> neighbor_chunk(recv_size);

        MPI_Recv(neighbor_chunk.data(), recv_size, MPI_INT, rank + step, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int current_local_size = local_chunk.size();
        local_chunk.insert(local_chunk.end(), neighbor_chunk.begin(),
                           neighbor_chunk.end());

        merge(local_chunk, 0, current_local_size - 1,
              (int)local_chunk.size() - 1);
      }
    } else {
      MPI_Send(local_chunk.data(), (int)local_chunk.size(), MPI_INT,
               rank - step, 0, MPI_COMM_WORLD);
      break;
    }
    step *= 2;
  }

  if (rank == 0) {
    cout << "Sorted array: ";
    for (int i = 0; i < chunk_size; i++) {
      cout << local_chunk[i] << " ";
    }
    cout << endl;
  }

  MPI_Finalize();
  return 0;
}
