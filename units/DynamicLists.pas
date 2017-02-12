unit DynamicLists;

{$MODE Delphi}

interface

uses LCLIntf, LCLType;

type
  TDynamicListItem = class
    Prev: TDynamicListItem;
    Next: TDynamicListItem;
    MyKey: DWord;
    MyItem: TObject;
  public
    constructor Create(Item: TObject; Key: Dword);
    destructor Destroy; override;
  end;

  TDynamicList = class(TObject)
    ItemsCount: Cardinal;
  private
    List: TDynamicListItem;
    Current: TDynamicListItem;
  public
    constructor Create;                     // Crea una lista di TObject.
    destructor Destroy; override;           // Distrugge la lista e i TObject.
    procedure Clear;                        // Distrugge tutti i TObject.
    procedure HeadInsert(Item: TObject);    // Inserisce in testa.
    procedure TailInsert(Item: TObject);    // Inserisce in coda.
    procedure KeyInsert(Item: Tobject; Key: DWord); // Inserimento per chiave.
    function First: TObject;                // Primo TObject o nil.
    function Next: TObject;                 // Prossimo TObject o nil.
    function Previous: TObject;             // Precedente TObject o nil.
    function Last: TObject;                 // Ultimo TObject o nil.
    function FindKey(Key: Dword): TObject;  // Cerca una chiave in particolare.
    procedure Unlink;                       // Distruggi TObject corrente.
  end;

implementation

constructor TDynamicListItem.Create(Item: TObject; Key: Dword);
begin
  Prev := Self;
  Next := Self;
  MyKey := Key;
  MyItem := Item;
end;

destructor TDynamicListItem.Destroy;
begin
  Prev.Next := Next;
  Next.Prev := Prev;
  if MyItem <> nil then
    MyItem.Destroy;
  inherited Destroy;
end;

//--------------------------------------------------------------------------//
// Crea una lista di oggetti vuota.                                         //
//--------------------------------------------------------------------------//
constructor TDynamicList.Create;
begin
  inherited Create;
  List := TDynamicListItem.Create(nil, 0);
  Current := List;
  ItemsCount := 0;
end;

//--------------------------------------------------------------------------//
// Distrugge una lista e tutti gli oggetti contenuti.                       //
//--------------------------------------------------------------------------//
destructor TDynamicList.Destroy;
begin
  while List.Next.MyItem <> nil do
    List.Next.Destroy;
  List.Destroy;  // Ultimo.
  inherited Destroy;
end;

//--------------------------------------------------------------------------//
// Svuota la lista di tutti gli oggetti, la posizione corrente diventa      //
// indefinita.                                                              //
//--------------------------------------------------------------------------//
procedure TDynamicList.Clear;
begin
  while List.Next.MyItem <> nil do
    List.Next.Destroy;
  Current := List;
  ItemsCount := 0;
end;

//--------------------------------------------------------------------------//
// Inserisce un oggetto in testa alla lista, la posizione corrente diventa  //
// quella dell'oggetto inserito. Lo inserisce con chiave 0.                 //
//--------------------------------------------------------------------------//
procedure TDynamicList.HeadInsert(Item: TObject);
begin
  if Item <> nil then
  begin
    Current := TDynamicListItem.Create(Item, 0);
    Current.Prev := List;
    Current.Next := List.Next;
    List.Next.Prev := Current;
    List.Next := Current;
    ItemsCount := ItemsCount + 1;
  end;
end;

//--------------------------------------------------------------------------//
// Inserisce un oggetto in coda alla lista, la posizione corrente diventa   //
// quella dell'oggetto inserito. Lo inserisce con chiave $ffffffff.         //
//--------------------------------------------------------------------------//
procedure TDynamicList.TailInsert(Item: TObject);
begin
  if Item <> nil then
  begin
    Current := TDynamicListItem.Create(Item, $ffffffff);
    Current.Next := List;
    Current.Prev := List.Prev;
    List.Prev.Next := Current;
    List.Prev := Current;
    ItemsCount := ItemsCount + 1;
  end;
end;

//--------------------------------------------------------------------------//
// Inserisce un oggetto nella lista, inserendolo per chiave in ordine       //
// crescente, la posizione corrente diventa quella dell'oggetto inserito.   //
//--------------------------------------------------------------------------//
procedure TDynamicList.KeyInsert(Item: Tobject; Key: DWord);
var
  pos: TDynamicListItem;
begin
  if Item <> nil then
  begin
    pos := List;
    while (pos.Next.MyItem <> nil) and (pos.Next.MyKey < Key) do
      pos := pos.Next;
    Current := TDynamicListItem.Create(Item, Key);
    Current.Prev := pos;
    Current.Next := pos.Next;
    pos.Next.Prev := Current;
    pos.Next := Current;
    ItemsCount := ItemsCount + 1;
  end;
end;

//--------------------------------------------------------------------------//
// Ritorna il primo oggetto della lista e si posiziona lì oppure nil se la  //
// lista è vuota.                                                           //
//--------------------------------------------------------------------------//
function TDynamicList.First: TObject;
begin
  Current := List.Next;
  result := Current.MyItem;
end;

//--------------------------------------------------------------------------//
// Ritorna il successivo oggetto della lista oppure nil se non esiste.      //
//--------------------------------------------------------------------------//
function TDynamicList.Next: TObject;
begin
  if Current.MyItem <> nil then
    Current := Current.Next;
  result := Current.MyItem;
end;

//--------------------------------------------------------------------------//
// Ritorna il precedente oggetto della lista oppure nil se non esiste.      //
//--------------------------------------------------------------------------//
function TDynamicList.Previous: TObject;
begin
  if Current.MyItem <> nil then
    Current := Current.Prev;
  result := Current.MyItem;
end;

//--------------------------------------------------------------------------//
// Ritorna l'ultimo oggetto della lista e si posiziona lì oppure nil se la  //
// lista è vuota.                                                           //
//--------------------------------------------------------------------------//
function TDynamicList.Last: TObject;
begin
  Current := List.Prev;
  result := Current.MyItem;
end;

//--------------------------------------------------------------------------//
// Si posiziona sul primo item con chiave uguale a quella indicata.         //
// Ritorna nil se non lo trova.                                             //
//--------------------------------------------------------------------------//
function TDynamicList.FindKey(Key: Dword): TObject;
var
  pos: TDynamicListItem;
begin
  pos := List.Next;
  while (pos.MyItem <> nil) and (pos.MyKey < Key) do
    pos := pos.Next;
  if (pos.MyItem <> nil) and (pos.MyKey = Key) then
  begin
    Current := pos;
    result := pos.MyItem
  end
  else
    result := nil;
end;

//--------------------------------------------------------------------------//
// Distrugge l'oggetto nella posizione corrente, la posizione corrente      //
// diventa indefinita.                                                      //
//--------------------------------------------------------------------------//
procedure TDynamicList.Unlink;
begin
  if Current.MyItem <> nil then
  begin
    Current.Destroy;
    Current := List;
    ItemsCount := ItemsCount - 1;
  end;
end;

end.
