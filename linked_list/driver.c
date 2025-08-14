#include<linux/init.h>
#include<linux/module.h>
#include<linux/list.h> // library for linked list in kernel

// This is my nodes structure, list_head contains next and prev pointers
struct Node
{
	struct list_head chain;
	int data;
};

// This is the entry point to my linkedlist called "myList". Initially its empty.
LIST_HEAD(myList);

int my_init(void)
{
	printk("Driver - Init\n");

	struct Node * newNode;

	// Creating a newNode
	newNode = kmalloc(sizeof(struct Node) , GFP_KERNEL);
	INIT_LIST_HEAD(&newNode->chain);
	newNode->data = 6;
	// Adding newNode to myList
	list_add(&newNode->chain , &myList);

	// Creating a newNode
	newNode = kmalloc(sizeof(struct Node) , GFP_KERNEL);
	INIT_LIST_HEAD(&newNode->chain);
	newNode->data = 12;
	// Adding newNode to myList
	list_add(&newNode->chain , &myList);

	// Creating a newNode
	newNode = kmalloc(sizeof(struct Node) , GFP_KERNEL);
	INIT_LIST_HEAD(&newNode->chain);
	newNode->data = 18;
	// Adding newNode to myList
	list_add(&newNode->chain , &myList);

	// Traversing the list
	struct list_head * ptr;
	list_for_each_reverse(ptr, &myList)
	{
		struct Node * temp = list_entry(ptr , struct Node , chain);
		printk("Driver - Element - %d\n", temp->data);
	}

	// Deleting the list
	struct Node * tempNode, * next;
	list_for_each_entry_safe(tempNode, next, &myList, chain)
	{
		list_del(&tempNode->chain);
		kfree(tempNode);
	}

	printk("Driver - Linked list deleted\n");

	return 0;
}


void my_exit(void)
{
	printk("Driver - Exit\n");
}


MODULE_LICENSE("GPL");
module_init(my_init);
module_exit(my_exit);
