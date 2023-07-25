# Storage

This is a container that stores records (strings) in sorted order.  
Records can be added and removed from the container. It also allows retrieving records by their ordinal number.

## API

The container provides the following API:

### Insert

To insert a new element into the container, use the `insert` method:

```cpp
container.insert(element);
```

* `element` (const std::string&): The element to be inserted into the container.
 
### Erase

To delete an element from the container by its ordinal number, use the `erase` method:

```cpp
container.erase(index);
```

* `index` (uint64_t): The ordinal number of the element to be deleted.

### Get

To get an element from the container by its ordinal number, use the `get` method:

```cpp
element = container.get(index);
```

* `index` (uint64_t): The ordinal number of the element to be retrieved.

## Usage Example

```cpp
int main()
{
	Storage st;

	st.insert("element_1");
	st.insert("element_2");
	st.insert("element_3");

	std::string element_1 = container.get(0);  # "element_1"
	std::string element_2 = container.get(1);  # "element_2"
	std::string element_3 = container.get(2);  # "element_3"

	container.delete(1);

	std::string element_1 = container.get(0);  # "element_1"
	std::string element_3 = container.get(1);  # "element_3"

	return 0;
}
```

## Performance

Performance of 1 Million Inputs with Sequential Million Deletions and Insertions:


