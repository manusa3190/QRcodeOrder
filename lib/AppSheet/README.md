
```cpp
#include <AppSheet.h>

AppSheet appsheet;

struct Item {
    int id;
    String name;
    int price;
};

void setup(){
  const char* APP_ID = "7b6b4413-ec3a-4d5d-b9d0-918c175e4487";
  const char* ACCESS_KEY = "V2-hapij-FMGbT-1rmp9-rEdZ6-BAF9C-UNfTv-mxMl1-gwaPa";
  appsheet.begin(APP_ID, ACCESS_KEY);

  Item items[3] = {
    {0, "アイテム1", 100},
    {0, "アイテム2", 200},
    {0, "アイテム3", 300},
  };

  const char* tableName = "hogeTable";

  // ADD
  appsheet.addItems(tableName, items, 3);

  // GET
  int itemCount;
  Item* fetchedItems = appsheet.getItems(tableName, "FILTER('備品マスタ', IN('実験室', [格納場所]))", itemCount);

  // UPDATE
  Item newItem = {fetchedItems[0].id, "アイテム1-1", 0}; // priceはそのまま
  Item updatedItem = appsheet.updateItem(tableName, newItem);

  // SET
  newItem = {fetchedItems[0].id, "アイテム1-2", 0}; // priceはnullになる
  Item setItem = appsheet.setItem(tableName, newItem);

  // DELETE
  appsheet.deleteItem(tableName, newItem);
  
  delete[] fetchedItems;
}

void loop() {
  // Nothing here
}

```