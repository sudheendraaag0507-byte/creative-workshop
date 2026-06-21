#  Rebuilding the STL: Linked List 

I got a vague idea of what a linked list is. It is nothing but a reincarnation of a dictionary in a more low-level language (at least, I kinda imagine it like that). Instead of using key-value pairs, each node uses its **VALUE** and a **POINTER** to link to the next data slot.

* **The Node:** Create an object which stores the value and the pointer to the next node.
* **The List Handler:** Add another object which helps us attach the nodes to the end of one another—let's call it `linkedList`.

---

##  Trashing the Edge Cases

When I completed this, I was curious about how the actual STL linked list worked (the code part). Honestly, it was really annoying to see how they used conditional statements just to avoid a single edge case—specifically, checking if the `headNode` is a null pointer. 

I was like, "I can just simply remove these conditional statements by introducing a **dummy node** at the start." 

Why invoke a separate method to store a value while creating a node when you can keep it streamlined? So yeah, here we are... Feel free to check out the branchless implementation!

#  Rebuilding the STL: Custom Contiguous Memory Allocator

Recently, I was too bored by rebuilding the STL from scratch and wanted to try something new. I realized modern linked lists are just nodes scattered across memory, causing multiple cache misses and making the life of the CPU miserably slow. 

Huh, I was like, "Bro, when things become more flexible, it is being traded with speed," and am not a big fan of low performance! To reduce cache misses and enhance performance, I looked at how the OS fetches memory—but the OS is not a big fan of scooping memory in order.

So I came up with an idea: Why don't I scoop out huge chunks of memory and stitch those pointers into an array of buffers? It absolutely beats the old uncontiguous traditional `malloc`. Ultimately, I was able to make a contiguous list on my custom memory allocator.

---

##  The Blueprint of the Machine

* **Contiguous Slots:** A custom-sized array of contiguous memory to work with, stored in a slot.
* **Dynamic Buffer:** A custom buffer to hold up the pointers to all these slots. This buffer is resizable if we run out of slots, without copying any of the data we stored in the memory into the new slots.

---

## Performance Breakdown

The theoretical worst time complexity is $O(n)$, but don't let this fool you. The time complexity is directly related to the size of each slot. If the size of the slot is high enough (e.g., in this machine it is 16Mb at max), the time complexity practically becomes $O(1)$. 

Feel free to take a look into the structure. Stay tuned for more of this content!
