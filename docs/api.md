# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`jungles`](#namespacejungles) | 
`struct `[`jungles::small_register::mask_not_matching_error`](#structjungles_1_1small__register_1_1mask__not__matching__error) | 
`struct `[`jungles::small_register::overflow_error`](#structjungles_1_1small__register_1_1overflow__error) | 
`struct `[`jungles::small_map::register_from_address`](#structjungles_1_1small__map_1_1register__from__address) | Performs the mapping at compile time. Use register_from_address::type alias to obtain the type.

# namespace `jungles` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`jungles::small_register`](#classjungles_1_1small__register) | Simplifies bitfield handling and adds safe checks. Bitfields are in Big Endian order.
`struct `[`jungles::bitfield`](#structjungles_1_1bitfield) | Describes a bitfield.
`struct `[`jungles::element`](#structjungles_1_1element) | Element of the map that relates the Address to a type SmallRegister which shall be a template instance of [jungles::small_register](#classjungles_1_1small__register).
`struct `[`jungles::small_map`](#structjungles_1_1small__map) | Maps values to specific [jungles::small_register](#classjungles_1_1small__register) template instances.

# class `jungles::small_register` 

Simplifies bitfield handling and adds safe checks. Bitfields are in Big Endian order.

#### Parameters
* `RegisterUnderlyingType` Underlying type of the register, which determines its size. Use e.g. uint8_t, uint16_t, uint32_t ... 

* `Bitfields` [jungles::bitfield](#structjungles_1_1bitfield) template instances that describe the layout of the register.

There are a few static assertions performed when instantiating the template:

* Bitfield IDs shall be unique. Compiler raises "Bitfield IDs must be unique" otherwise.

* The bitfields shall give in total the bitsize of the RegisterUnderlyingType bitsize. Otherwise compiler raises "Whole register must be allocated" error. If there are unused bits then use e.g. bitfield<reg1::unused, 3>

* The types of bitfield IDs shall be the same. So using various "enum class" types will not work. Otherwise compiler raises "bitfield::id types shall be the same".

* When using methods a template parameter which is the bitfield ID is needed. When specyfying wrong ID (one which was not registered through template instantiation then a compiler error is thrown: "Bitfield ID not found"

All the mutating methods like [set()](#classjungles_1_1small__register_1a0b8b50e42b9160367cd0638b11b199df), [clear()](#classjungles_1_1small__register_1ac4488087ee6ee45a59215fe6692d0866), ... return reference to self so the mutating opperations can be chained: "r.set<id1>().clear<id2>().set<id3>()"

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline constexpr `[`small_register`](#classjungles_1_1small__register_1a3a1b936f00c55edc595409b43964000f)`(Register initial_value)` | Constructs the register with initial_value that is mapped to the defined bitfields. Initial value is zero if not specified.
`public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`set`](#classjungles_1_1small__register_1a0b8b50e42b9160367cd0638b11b199df)`()` | Sets all the bits of the bitfield to ones.
`public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`set`](#classjungles_1_1small__register_1a38189b0d7ced4a328be82a985e09a049)`(RegisterUnderlyingType value)` | Sets the bitfield to a specified value, that is equivalent to "\|= value" operation on the bitfield.
`public template<>`  <br/>`inline constexpr RegisterUnderlyingType `[`get`](#classjungles_1_1small__register_1aa8d6d598082926ac85c569b0b3a1c468)`()` | Returns the value of the specified bitfield.
`public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`clear`](#classjungles_1_1small__register_1ac4488087ee6ee45a59215fe6692d0866)`()` | Clears the whole bitfield (sets all bits to zeros).
`public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`clear`](#classjungles_1_1small__register_1a0360d9ae8a58fa4c3afab1a6facd4f98)`(RegisterUnderlyingType mask)` | Clears the bitfield applying the mask. That is equivalent to "&= ~(mask)" operation on the bitfield.
`public inline constexpr RegisterUnderlyingType `[`operator()`](#classjungles_1_1small__register_1a687f7954d4635435981df244456a1618)`() const` | Returns the underlying value.

## Members

#### `public inline constexpr `[`small_register`](#classjungles_1_1small__register_1a3a1b936f00c55edc595409b43964000f)`(Register initial_value)` 

Constructs the register with initial_value that is mapped to the defined bitfields. Initial value is zero if not specified.

#### `public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`set`](#classjungles_1_1small__register_1a0b8b50e42b9160367cd0638b11b199df)`()` 

Sets all the bits of the bitfield to ones.

#### `public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`set`](#classjungles_1_1small__register_1a38189b0d7ced4a328be82a985e09a049)`(RegisterUnderlyingType value)` 

Sets the bitfield to a specified value, that is equivalent to "|= value" operation on the bitfield.

#### Exceptions
* `[overflow_error](#structjungles_1_1small__register_1_1overflow__error)` when value is bigger than the maximum value the bitfield can store.

#### `public template<>`  <br/>`inline constexpr RegisterUnderlyingType `[`get`](#classjungles_1_1small__register_1aa8d6d598082926ac85c569b0b3a1c468)`()` 

Returns the value of the specified bitfield.

#### `public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`clear`](#classjungles_1_1small__register_1ac4488087ee6ee45a59215fe6692d0866)`()` 

Clears the whole bitfield (sets all bits to zeros).

#### `public template<>`  <br/>`inline constexpr `[`Self`](#classjungles_1_1small__register)` & `[`clear`](#classjungles_1_1small__register_1a0360d9ae8a58fa4c3afab1a6facd4f98)`(RegisterUnderlyingType mask)` 

Clears the bitfield applying the mask. That is equivalent to "&= ~(mask)" operation on the bitfield.

#### Exceptions
* `[mask_not_matching_error](#structjungles_1_1small__register_1_1mask__not__matching__error)` when mask is bigger than the maximum value the bitfield can store.

#### `public inline constexpr RegisterUnderlyingType `[`operator()`](#classjungles_1_1small__register_1a687f7954d4635435981df244456a1618)`() const` 

Returns the underlying value.

# struct `jungles::bitfield` 

Describes a bitfield.

Must be used as an input to jungles:[small_register](#classjungles_1_1small__register) template instantiation. 

#### Parameters
* `Id` Should be an ID which is a number or enumeration. 

* `Size` Bit-size of the bitfield.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `jungles::element` 

Element of the map that relates the Address to a type SmallRegister which shall be a template instance of [jungles::small_register](#classjungles_1_1small__register).

Must be used as an input to jungles:[small_map](#structjungles_1_1small__map) template instantiation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`typedef `[`Register`](#structjungles_1_1element_1ab398c1215d126c6e40a16f591359509a) | 

## Members

#### `typedef `[`Register`](#structjungles_1_1element_1ab398c1215d126c6e40a16f591359509a) 

# struct `jungles::small_map` 

Maps values to specific [jungles::small_register](#classjungles_1_1small__register) template instances.

#### Parameters
* `Elements` Multiple [jungles::element](#structjungles_1_1element) which are elements of the map.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `jungles::small_register::mask_not_matching_error` 

```
struct jungles::small_register::mask_not_matching_error
  : public exception
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `jungles::small_register::overflow_error` 

```
struct jungles::small_register::overflow_error
  : public exception
```  

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `jungles::small_map::register_from_address` 

Performs the mapping at compile time. Use register_from_address::type alias to obtain the type.

#### Parameters
* `Register` address (the value) that is key to obtain the type for that register address.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`typedef `[`type`](#structjungles_1_1small__map_1_1register__from__address_1aad4172a7a4ff6cc6c403b49c4c571c0f) | 

## Members

#### `typedef `[`type`](#structjungles_1_1small__map_1_1register__from__address_1aad4172a7a4ff6cc6c403b49c4c571c0f) 

Generated by [Moxygen](https://sourcey.com/moxygen)