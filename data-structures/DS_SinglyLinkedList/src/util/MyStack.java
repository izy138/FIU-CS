package util;

public class MyStack {
        private static class Node<T> {
            T data;
            public Node<T> next;

            private Node(T data) {
                this.data = data;
            }

            private Node<T> top;

            public boolean isEmpty() {
                return top == null;
            }

            public T peek() {
                return top.data;
            }

            public void push(T data) {
                Node node = new Node(data);
                node.next = top;
                top = node;
            }
            public T pop(){
                T data = top.data;
                top = top.next;
                return data;
            }
        }
    }


