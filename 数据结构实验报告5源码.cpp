#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct huffman_node {
	char symbol;
	int count;
	struct huffman_node *left, *right;
} huffnode, *huffptr;

struct wcnm {
	char shunxu;
	int count;
};

// 创建哈夫曼树的节点
huffptr create_huffman_node(char symbol, int count) {
	huffptr node = (huffptr)malloc(sizeof(huffnode));
	node->symbol = symbol;
	node->count = count;
	node->left = node->right = NULL;
	return node;
}

// 通过两个最小的节点创建一个新的节点
huffptr create_internal_node(huffptr left, huffptr right) {
	huffptr node = create_huffman_node('\0', left->count + right->count);
	node->left = left;
	node->right = right;
	return node;
}

// 构建哈夫曼树
huffptr build_huffman_tree(struct wcnm *arr, int length) {
	int current_size = length;
	huffptr nodes[length];
	
	// 初始化节点数组
	for (int i = 0; i < length; ++i) {
		nodes[i] = create_huffman_node(arr[i].shunxu, arr[i].count);
	}
	while (current_size > 1) {
		int min1 = 0, min2 = 1;
		
		// 找到最小和次小的节点
		for (int i = 2; i < current_size; ++i) {
			if (nodes[i]->count < nodes[min1]->count) {
				min2 = min1;
				min1 = i;
			} else if (nodes[i]->count < nodes[min2]->count) {
				min2 = i;
			}
		}
		
		// 创建新的内部节点
		huffptr new_node = create_internal_node(nodes[min1], nodes[min2]);
		
		// 将合并的节点放入数组最后，并减小数组大小
		nodes[min1] = new_node;
		nodes[min2] = nodes[current_size - 1];
		current_size--;
	}
	
	// 返回根节点
	return nodes[0];
}

int is_char_already_appeared(const char *str, int n) {
	// 遍历字符串前面的字符
	for (int i = 0; i < n; ++i) {
		// 检查当前字符是否与 str[n] 相等
		if (str[i] == str[n]) {
			return 1; // 字符已经出现过
		}
	}
	return 0; // 字符未出现过
}

void bubble_sort(struct wcnm *arr, int length) {
	for (int i = 0; i < length - 1; ++i) {
		for (int j = 0; j < length - i - 1; ++j) {
			if (arr[j].count < arr[j + 1].count) {
				// Swap array elements
				struct wcnm temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void print_huffman_codes(huffptr root, char code[], int top) {
	if (root->left) {
		code[top] = '0';
		print_huffman_codes(root->left, code, top + 1);
	}
	
	if (root->right) {
		code[top] = '1';
		print_huffman_codes(root->right, code, top + 1);
	}
	
	if (!root->left && !root->right) {
		printf(" %c    ", root->symbol);
		for (int i = 0; i < top; ++i) {
			printf("%c", code[i]);
		}
		printf("\n");
	}
}

int main() {
	char str[1024];
	struct wcnm wcnm[1024];
	int length;
	
	printf("请输入要编码的字符串：");
	fgets(str, sizeof(str), stdin);
	length = strlen(str);
	if (length > 0 && str[length - 1] == '\n') {
		// 将换行符替换为 null 终止符
		str[length - 1] = '\0';
	}
	length = strlen(str);
	
	int w = 0;
	for (int i = 0; i < length; ++i) {
		if (!is_char_already_appeared(str, i)) {
			wcnm[w].shunxu = str[i];
			wcnm[w].count = 0;
			
			for (int j = i; j < length-1; ++j) {
				if (str[j] == str[i]) {
					wcnm[w].count++;
				}
			}
			
			w++;
		}
	}
	
	// Sort wcnm array based on count
	bubble_sort(wcnm, w-1);
	
	// Print sorted characters and frequencies
//	for (int i = 0; i < w-1; ++i) {
//		printf("%c  %d\n", wcnm[i].shunxu, wcnm[i].count);
//	}
	// 构建哈夫曼树
	huffptr huffman_tree = build_huffman_tree(wcnm, w-1);
	
	char code[100];
	print_huffman_codes(huffman_tree, code, 0);
	free(huffman_tree);
	
	return 0;
}

