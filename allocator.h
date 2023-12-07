//
// Created by aliye on 3/18/2023.
//

#ifndef LABWORK_9_SCANERIST__ALLOCATOR_H_
#define LABWORK_9_SCANERIST__ALLOCATOR_H_

#include <iostream>
template<typename T>
class allocator {
 public:
  using value_type = T;
  class pool {
   public:
    pool(int size, int num) {
        sizeOfchuck_ = size;
        numOfChucks_ = num;
        byte_ = static_cast<char *>(std::malloc(size * num));
        UsedChuncks_ = new bool[num];
        for (int i = 0; i < num; ++i) {
            UsedChuncks_[i] = false;
        }
    }
    int get_size_of_chunck() {
        return sizeOfchuck_;
    }
    char *get_byte() {
        return byte_;
    }
    bool *get_used_chuncks() {
        return UsedChuncks_;
    }
    int get_num_of_chunck() {
        return numOfChucks_;
    }

    char *get_chunk(int num) {
        int chunkIndex = -1;
        int chunkCount = 0;
        for (int i = 0; i < numOfChucks_; ++i) {
            if (!UsedChuncks_[i]) {
                if (chunkIndex == -1) {
                    chunkIndex = i;
                }
                if (++chunkCount == num) {
                    for (int j = chunkIndex; j < chunkIndex + num; ++j) {
                        UsedChuncks_[j] = true;
                    }
                    return &byte_[chunkIndex * sizeOfchuck_];
                }
            } else {
                chunkIndex = -1;
                chunkCount = 0;
            }
        }
        return nullptr;
    }

    void free_chuncks(const char* first_cell, int count) {
        if (first_cell >= byte_ && first_cell < byte_ + numOfChucks_ * sizeOfchuck_) {
            int start_index = (first_cell - byte_) / sizeOfchuck_;
            int end_index = start_index + count;
            for (int i = start_index; i < end_index && i < numOfChucks_; i++) {
                UsedChuncks_[i] = false;
            }
        }
    }

   private:
    int numOfChucks_;
    int sizeOfchuck_;
    bool *UsedChuncks_;
    char *byte_;
  };

  allocator(int pools,  const int* chunks_per_pool,  const int *chunks_size) {
      num_of_pools = pools;
      Pools = new pool * [num_of_pools];
      for(int i = 0; i < num_of_pools; i++){
          Pools[i] = new pool(chunks_size[i], chunks_per_pool[i]);
      }
  }

  constexpr T *allocate(int size) {
      for (int i = 0; i < num_of_pools; ++i) {
          pool* p = Pools[i];
          int chunk_size = p->get_size_of_chunck();
          bool* used_chunks = p->get_used_chuncks();
          int num_required_chunks = size * sizeof(T) / chunk_size + ((size * sizeof(T) % chunk_size) ? 1 : 0);
          char* chunk_start = p->get_chunk(num_required_chunks);
          if (chunk_start) {
              int chunk_index = (chunk_start - p->get_byte()) / chunk_size;
              for (int j = 0; j < num_required_chunks; ++j) {
                  used_chunks[chunk_index + j] = true;
              }
              return reinterpret_cast<T*>(chunk_start);
          }
      }
      throw std::bad_alloc{};
  }

  ~allocator() {
      std::_Destroy_n(Pools, num_of_pools);
  }

  void deallocate(T* ptr, int size) {
      for (int i = 0; i < num_of_pools; ++i) {
          pool* p = Pools[i];
          char* chunk_start = reinterpret_cast<char*>(ptr);
          int chunk_size = p->get_size_of_chunck();
          char* pool_start = p->get_byte();
          char* pool_end = pool_start + p->get_num_of_chunck() * chunk_size;

          if (chunk_start >= pool_start && chunk_start < pool_end) {
               int64_t chunk_index = (chunk_start - pool_start) / chunk_size;
              p->free_chuncks(pool_start + chunk_index * chunk_size, 1);
              return;
          }
      }
  }


 private:
  int num_of_pools;
  pool **Pools;

};

#endif //LABWORK_9_SCANERIST__ALLOCATOR_H_
