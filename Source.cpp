#include<iostream>

struct backup {
	size_t size;
	bool is_free;
};


struct node_t {
	size_t size;
	bool is_free;
	node_t* next;
	backup* prev;
	backup* back;
};


node_t* head = nullptr;
size_t _size;
void* head_2;
void* end;

void mysetup(void* _head, size_t size) {
	_size = size;
	head_2 = _head;
	end = (void*)((char*)(_head)+size);
}

node_t* heap() {
	if (head == nullptr) {
		head = (node_t*)head_2;
		head->size = _size - sizeof(node_t) - sizeof(backup);
		head->is_free = true;
		head->back = (backup*)(((char*)head) + _size - sizeof(backup)); //+ _size - sizeof(backup)));
		head->back->is_free = true;
		head->back->size = head->size;
		head->next = nullptr;
		head->prev = nullptr;
	}
	return head;
}



void find(size_t size, node_t** free_area) {
	size_t actual_size = sizeof(node_t) + size + sizeof(backup);
	node_t* curr = heap();

	while (curr != nullptr) {

		if (curr->size >= actual_size && curr->is_free == true) {
			*free_area = curr;
			break;
		}
		curr = curr->next;
	}
}



void split(size_t size, node_t** found, node_t** allocated) {
	node_t* main_head = head;
	node_t* curr = *found;
	size_t actual_size = sizeof(node_t) + size + sizeof(backup);
	size_t offset = (curr->size + sizeof(node_t) - actual_size);

	curr = (node_t*)(((char*)*found) + offset + sizeof(backup));
	curr->size = size;
	curr->is_free = false;
	curr->back = (*found)->back;
	curr->back->size = size;
	curr->back->is_free = false;
	curr->next = (*found)->next;


	(*found)->size = offset - sizeof(node_t);
	(*found)->back = (backup*)(((char*)*found) + offset);
	(*found)->back->is_free = true;
	(*found)->back->size = (*found)->size;
	(*found)->next = curr;


	curr->prev = (*found)->back;


	*allocated = curr;

}



void* myalloc(size_t size) {
	if (size == 0) { return nullptr; }
	node_t* found = nullptr;
	node_t* allocated = nullptr;
	find(size, &found);
	// if found is NULL aka no available memory
	if (found == nullptr) {
		return nullptr;
	}
	split(size, &found, &allocated);

	// adjust the pointer to right after the allocated header_t
	return ((char*)allocated + sizeof(node_t));
}


void cheak_neib(node_t** free_block) {


	bool converted = false;

	if ((*free_block)->next == nullptr && (*free_block)->prev == nullptr) {
		return;
	}

	else if ((*free_block)->next == nullptr && (*free_block)->prev != nullptr) {


		if ((*free_block)->prev->is_free == true) {
			node_t* curr = *free_block;
			(*free_block) = (node_t*)(((char*)(*free_block)->prev) - (*free_block)->prev->size - sizeof(node_t));
			(*free_block)->size += curr->size + sizeof(node_t) + sizeof(backup);

			(*free_block)->back = curr->back;
			(*free_block)->back->size = (*free_block)->size;
			(*free_block)->back->is_free = true;
			(*free_block)->is_free = true;
			(*free_block)->next = curr->next;

			converted = true;
		}


		if (converted == false) {
			(*free_block)->is_free = true;
			(*free_block)->back->is_free = true;
		}

	}

	else if ((*free_block)->prev == nullptr && (*free_block)->next != nullptr) {

		if ((*free_block)->next->is_free == true) {
			(*free_block)->is_free = true;
			(*free_block)->back = (*free_block)->next->back;
			(*free_block)->size += (*free_block)->next->size + sizeof(node_t) + sizeof(backup);
			(*free_block)->back->size = (*free_block)->size;
			(*free_block)->back->is_free = true;
			(*free_block)->next = (*free_block)->next->next;

			converted = true;
		}

		if (converted == false) {
			(*free_block)->is_free = true;
			(*free_block)->back->is_free = true;
		}


		_abs64;
	}



	else {

		if ((*free_block)->prev->is_free == true) {
			node_t* curr = *free_block;
			(*free_block) = (node_t*)(((char*)(*free_block)->prev) - (*free_block)->prev->size - sizeof(node_t));
			(*free_block)->size += curr->size + sizeof(node_t) + sizeof(backup);

			(*free_block)->back = curr->back;
			(*free_block)->back->size = (*free_block)->size;
			(*free_block)->back->is_free = true;
			(*free_block)->is_free = true;
			(*free_block)->next = curr->next;

			converted = true;
		}

		if ((*free_block)->next->is_free == true) {
			(*free_block)->is_free = true;
			(*free_block)->back = (*free_block)->next->back;
			(*free_block)->size += (*free_block)->next->size + sizeof(node_t) + sizeof(backup);
			(*free_block)->back->size = (*free_block)->size;
			(*free_block)->back->is_free = true;
			(*free_block)->next = (*free_block)->next->next;

			converted = true;
		}

		if (converted == false) {
			(*free_block)->is_free = true;
			(*free_block)->back->is_free = true;
		}
	}

}


void myfree(void* p) {

	node_t* free = (node_t*)(((char*)p) - sizeof(node_t));

	if (free->is_free == true) return;
	cheak_neib(&free);


}

int main() {
	void* pointer = (void*)malloc(1000);
	mysetup(pointer, 1000);

	void* test_1 = myalloc(10);
	void* test_2 = myalloc(10);
	void* test_3 = myalloc(10);
	void* test_4 = myalloc(10);
	void* test_5 = myalloc(10);

	std::cout << test_1 << ' ' << test_2 << '\n';

	node_t* test_1_addr = (node_t*)(((char*)test_1) - sizeof(node_t));
	node_t* test_2_addr = (node_t*)(((char*)test_2) - sizeof(node_t));
	std::cout << test_1_addr->prev << ' '<< test_2_addr->next;

	//myfree(test_2);
	//myfree(test_4);
	//myfree(test_3);

	free(pointer);

	return 0;
}