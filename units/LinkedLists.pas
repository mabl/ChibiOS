//
// Gestione generica di liste linkate.
//
unit LinkedLists;

{$MODE Delphi}

interface

type
  TBaseSingleListItem = class(TObject)
  public
    Next: TBaseSingleListItem;
    constructor Create;
    function IsEmpty: Boolean;
    procedure Link(Item: TBaseSingleListItem);
    function UnlinkNext: TBaseSingleListItem;
  end;

  TAbstractDeltaListItem = class(TBaseSingleListItem)
  public
    Delta: Cardinal;
    procedure TimeElapsed; virtual; abstract;
  end;

  TDeltaListCallback = procedure(Obj: TObject; Data: Pointer);

  TDeltaListItem = class(TAbstractDeltaListItem)
  private
    MyData: Pointer;
    MyObj: TObject;
    MyCallback: TDeltaListCallback;
  public
    constructor Create(Callback: TDeltaListCallback; Obj: TObject; Data: Pointer);
    procedure TimeElapsed; override;
  end;

  TDeltaList = class(TAbstractDeltaListItem)
  private
    procedure DeltaInsert(Time: Cardinal; Item: TAbstractDeltaListItem);
  public
    constructor Create;
    procedure Insert(Time: Cardinal;
                     Callback: TDeltaListCallback;
                     Obj: TObject;
                     Data: Pointer); overload;
    procedure Insert(Time: Cardinal; Item: TAbstractDeltaListItem); overload;
    procedure CountTimeUnit;
  end;

  TBaseDoubleListItem = class(TObject)
  public
    Next: TBaseDoubleListItem;
    Prev: TBaseDoubleListItem;
    constructor Create;
    function IsEmpty: Boolean;
    procedure Unlink;
    procedure LinkNext(Item: TBaseDoubleListItem);
    procedure LinkPrev(Item: TBaseDoubleListItem);
  end;

  TDynamicObjectListItem = class(TBaseDoubleListItem)
  public
    MyObj: TObject;
    constructor Create(Obj: Tobject);
  end;

  TDynamicObjectList = class(TBaseDoubleListItem)
  private
    Current: TBaseDoubleListItem;
  public
    constructor Create;
    destructor Destroy; override;
    procedure InsertFirst(Obj: Tobject);
    procedure InsertLast(Obj: Tobject);
    procedure InsertNext(Obj: Tobject);
    procedure InsertPrev(Obj: Tobject);
    function CurrentObject: TObject;
    function FirstObject: TObject;
    function NextObject: TObject;
    function LastObject: TObject;
    function PrevObject: TObject;
    function UnlinkObject: TObject;
  end;

implementation

//------------------------------------------------------------------------//
// Liste a singolo link generiche.                                        //
//------------------------------------------------------------------------//
constructor TBaseSingleListItem.Create;
begin
  inherited Create;
  Next := self;
end;

function TBaseSingleListItem.IsEmpty: Boolean;
begin
  if Next = self then
    result := True
  else
    result := False;
end;

procedure TBaseSingleListItem.Link(Item: TBaseSingleListItem);
begin
  Item.Next := Next;
  Next := Item;
end;

function TBaseSingleListItem.UnlinkNext: TBaseSingleListItem;
begin
  result := Next;
  Next := Next.Next;
end;

//------------------------------------------------------------------------//
// Liste delta con varie modalità (collezione, derivazione e callback).   //
//------------------------------------------------------------------------//
constructor TDeltaListItem.Create(Callback: TDeltaListCallback;
                                  Obj: TObject;
                                  Data: Pointer);
begin
  inherited Create;
  MyData := Data;
  MyObj := Obj;
  MyCallback := Callback;
end;

procedure TDeltaListItem.TimeElapsed;
begin
  MyCallback(MyObj, MyData);
  Destroy;
end;

constructor TDeltaList.Create;
begin
  inherited Create;
  Delta := $ffffffff;
end;

procedure TDeltaList.DeltaInsert(Time: Cardinal; Item: TAbstractDeltaListItem);
var
  curr: TAbstractDeltaListItem;
begin
  curr := self;
  while TAbstractDeltaListItem(curr.Next).Delta < Time do
  begin
    Time := Time - curr.Delta;
    curr := TAbstractDeltaListItem(curr.Next);
  end;
  if curr.Next <> self then
    TAbstractDeltaListItem(curr.Next).Delta := TAbstractDeltaListItem(curr.Next).Delta - Time;
  Item.Delta := Time;
  curr.Link(Item);
end;

//
// Chiama "Callback" dopo il tempo specificato, passando l'oggetto "Obj"
// e/o il puntatore "Data".
//
procedure TDeltaList.Insert(Time: Cardinal;
                            Callback: TDeltaListCallback;
                            Obj: TObject;
                            Data: Pointer);
begin
  DeltaInsert(Time, TDeltaListItem.Create(Callback, Obj, Data));
end;

//
// Chiama il metodo "TimeElapsed" dell'oggetto "Item" dopo il tempo
// specificato (va derivata una classe da TAbstractDeltaListItem).
//
procedure TDeltaList.Insert(Time: Cardinal; Item: TAbstractDeltaListItem);
begin
  DeltaInsert(Time, Item);
end;

//
// Contatore del tempo, da richiamare ad intervalli per aggiornare la delta list.
//
procedure TDeltaList.CountTimeUnit;
begin
  if not IsEmpty then
  begin
    TAbstractDeltaListItem(Next).Delta := TAbstractDeltaListItem(Next).Delta - 1;
    while TAbstractDeltaListItem(Next).Delta = 0 do
      TAbstractDeltaListItem(UnlinkNext).TimeElapsed;
  end;
end;

//------------------------------------------------------------------------//
// Liste a doppio link generiche.                                         //
//------------------------------------------------------------------------//
constructor TBaseDoubleListItem.Create;
begin
  inherited Create;
  Next := self;
  Prev := self;
end;

function TBaseDoubleListItem.IsEmpty: Boolean;
begin
  if Prev = Next then
    result := True
  else
    result := False;
end;

procedure TBaseDoubleListItem.Unlink;
begin
  self.Next.Prev := self.Prev;
  self.Prev.Next := self.Next;
end;

procedure TBaseDoubleListItem.LinkNext(Item: TBaseDoubleListItem);
begin
  Item.Next := self.Next;
  Item.Prev := self;
  self.Next.Prev := Item;
  self.Next := Item;
end;

procedure TBaseDoubleListItem.LinkPrev(Item: TBaseDoubleListItem);
begin
  Item.Prev := self.Prev;
  Item.Next := self;
  self.Prev.Next := Item;
  self.Prev := Item;
end;

//------------------------------------------------------------------------//
// Liste bidirezionali di TObject.                                        //
//------------------------------------------------------------------------//
constructor TDynamicObjectListItem.Create(Obj: Tobject);
begin
  inherited Create;
  MyObj := Obj;
end;

constructor TDynamicObjectList.Create;
begin
  inherited Create;
  Current := self;
end;

destructor TDynamicObjectList.Destroy;
begin
  while FirstObject <> nil do
    UnlinkObject.Destroy;
  inherited;
end;

procedure TDynamicObjectList.InsertFirst(Obj: Tobject);
begin
  LinkNext(TDynamicObjectListItem.Create(Obj));
end;

procedure TDynamicObjectList.InsertLast(Obj: Tobject);
begin
  LinkPrev(TDynamicObjectListItem.Create(Obj));
end;

procedure TDynamicObjectList.InsertNext(Obj: Tobject);
begin
  Current.LinkNext(TDynamicObjectListItem.Create(Obj));
end;

procedure TDynamicObjectList.InsertPrev(Obj: Tobject);
begin
  Current.LinkPrev(TDynamicObjectListItem.Create(Obj));
end;

function TDynamicObjectList.CurrentObject: TObject;
begin
  if Current <> self then
    result := TDynamicObjectListItem(Current).MyObj
  else
    result := nil;
end;

function TDynamicObjectList.FirstObject: TObject;
begin
  Current := Next;
  result := CurrentObject;
end;

function TDynamicObjectList.NextObject: TObject;
begin
  if Current <> self then
  begin
    Current := Current.Next;
    result := CurrentObject;
  end
  else
    result := nil;
end;

function TDynamicObjectList.LastObject: TObject;
begin
  Current := Prev;
  result := CurrentObject;
end;

function TDynamicObjectList.PrevObject: TObject;
begin
  if Current <> self then
  begin
    Current := Current.Prev;
    result := CurrentObject;
  end
  else
    result := nil;
end;

function TDynamicObjectList.UnlinkObject: TObject;
var
  curr: TDynamicObjectListItem;
begin
  if Current <> self then
  begin
    curr := TDynamicObjectListItem(Current);
    Current := Current.Next;
    result := curr.MyObj;
    curr.Destroy;
  end
  else
    result := nil;
end;

end.
