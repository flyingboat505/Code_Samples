from abc import ABC, abstractmethod
import sys
from datetime import datetime
from collections import defaultdict
class Quote:
    def __init__(self, id, symbol, price, available_volume,
                 expiration_datetime):
        self.id = id
        self.symbol = symbol
        self.price = price
        self.available_volume = available_volume
        self.expiration_datetime = expiration_datetime


class TradeResult:
    def __init__(self, id, symbol, volume_weighted_average_price,
                 volume_requested, volume_executed):
        self.id = id
        self.symbol = symbol
        self.volume_weighted_average_price = volume_weighted_average_price
        self.volume_requested = volume_requested
        self.volume_executed = volume_executed


class AbstractQuoteManager(ABC):
    @abstractmethod
    def add_or_update_quote(self, quote):
        """
        Add or update the quote (specified by Id) in symbol's book.
        If quote is new or no longer in the book, add it. Otherwise update it
        to match the given price, volume, and symbol.
        """
        pass

    @abstractmethod
    def remove_quote(self, id):
        """
        Remove quote by Id, if quote is no longer in symbol's book do nothing.
        """
        pass

    @abstractmethod
    def remove_all_quotes(self, symbol):
        """
        Remove all quotes on the specifed symbol's book.
        """
        pass

    @abstractmethod
    def get_best_quote_with_available_volume(self, symbol) -> Quote:
        """
        Get the best (i.e. lowest) price in the symbol's book that still has
        available volume.  If there is no quote on the symbol's book with
        available volume, return null.  Otherwise return a Quote object with
        all the fields set.  Don't return any quote which is past its
        expiration time, or has been removed.
        """
        pass

    @abstractmethod
    def execute_trade(self, symbol, volume_requested) -> TradeResult:
        """
        Request that a trade be executed. For the purposes of this interface,
        assume that the trade is a request to BUY, not sell. Do not trade an
        expired quotes.
        To Execute a trade:
        * Search available quotes of the specified symbol from best price to
        worst price.
        * Until the requested volume has been filled, use as much available
        volume as necessary (up to what is available) from each quote,
        subtracting the used amount from the available amount.

        For example, we have two quotes:
        #>>> {Price: 1.0, Volume: 1,000, AvailableVolume: 750}
        #>>> {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}

        After calling once for 500 volume, the quotes are:
       # >>> {Price: 1.0, Volume: 1,000, AvailableVolume: 250}
      3  >>> {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}

        And After calling this a second time for 500 volume, the quotes are:
       # >>> {Price: 1.0, Volume: 1,000, AvailableVolume: 0}
       # >>> {Price: 2.0, Volume: 1,000, AvailableVolume: 750}
        """
        pass


def Sort_Tuple(tup):
    # reverse = None (Sorts in Ascending order)
    # key is set to sort using second element of
    # sublist lambda has been used
    tup.sort(key=lambda x: x[1])
    return tup

class QuoteManager():
    #create two has tables: map1 and map2.
    def __init__(self):
        self.map_1 = {}  # Map1 has key ID and Value, Symbol
        self.map_2 = {}  # May2 has key symbol and value array list
    #check if id in hash table else print out fake

    def printTradeResult(self, trade_result):
        if (trade_result == None):
            return None
        print("id", trade_result.id)
        print("symbol", trade_result.symbol)
        print("volume_weighted_average_price", trade_result.volume_weighted_average_price)
        print("volume_requested", trade_result.volume_requested)
        print("volume_executed", trade_result.volume_executed)

    def printObject(self,quote):
        if(quote==None):
            return None
        else:
            print("symbol", quote.symbol)
            print("id",quote.id)
            print("available_volume", quote.available_volume)
            print("expiration_datetime", quote.expiration_datetime)
            print("price", quote.price)
    def printMaps(self):
        print("Map 1")
        [print("ID",key,"Symbol",value) for key, value in self.map_1.items()]
        print("Map 2")
        for key, values in self.map_2.items():
            for v in values:
                print("Symbol",key, " Object ", v , "id",v.id,"symbol",v.symbol,"price ",v.price,
                      "available_volume", v.available_volume,"expiration_datetime",v.expiration_datetime)

    def add_or_update_quote(self, quote):
        #edge case, value of book is found. (I'm assuming that ID doesn't change symbols if it already exists in a symbol book)
        if(self.map_1.get(quote.id) is not None and self.map_1.get(quote.id)!=quote.symbol):
            print("EDGE CASE:ID already exists inside a symbol table, please match the symbol",self.map_1.get(quote.id))
            return

        self.map_1[quote.id] =quote.symbol #add symbol value I to map 1(update shouldn't make a difference)
        if self.map_2.get(quote.symbol) is None:
            self.map_2[quote.symbol]=[quote]
        else:
            index=0
            flag=0
            for quote_object in self.map_2[quote.symbol]:
                if quote_object.id==quote.id: #found, update the quote object
                    #quote_object.volume_weighted_average_price=quote.volume_weighted_average_price
                    #quote_object.volume_requested = quote.volume_requested
                    #quote_object.volume_executed = quote.volume_executed
                    (self.map_2[quote.symbol])[index].price=quote.price
                    (self.map_2[quote.symbol])[index].available_volume = quote.available_volume
                    (self.map_2[quote.symbol])[index].expiration_datetime = quote.expiration_datetime
                    flag=1
                index=index+1

            if flag==0: #object in list
                self.map_2[quote.symbol].append(quote)
    def remove_quote(self, id):
        if(self.map_1.get(id) is None):
            print("No ID exists")
            return
        else:
            #remove from Map2
            # remove from Map1
            symbol=self.map_1[id]
            for quote_object in self.map_2[symbol]:
                if quote_object.id == id:
                    (self.map_2[symbol]).remove(quote_object )
            del self.map_1[id]
    def remove_all_quotes(self, symbol):
        if(self.map_2.get(symbol) is None):
            print("No SYMBOL exists")
            return
        else:
            list=[]
            for id in self.map_1:
                if self.map_1[id]==symbol:
                    list.append(id)
            for id in list:
                del self.map_1[id]
            self.map_2[symbol]=[]
            return

    def get_best_quote_with_available_volume(self, symbol) -> Quote:
        #for loop, get lowest price(provided that it still has avaible volume), with respective index
        if (self.map_2.get(symbol) is None):
            print("No SYMBOL exists, RETURNING NONE")
            return None
        else:
            currentYear = datetime.now().year #edit would've used more specific dates and time if I had a clear idea of the data format, see email
            lowest_index=-1
            lowest_price=sys.maxsize
            index=0
            for quote_object in self.map_2[symbol]:
                if quote_object.price < lowest_price and quote_object.expiration_datetime > currentYear:  # found, update the quote object
                   lowest_price=quote_object.price
                   lowest_index=index
                index = index + 1
            if lowest_index==-1:
                return None
            else:
                answer= (self.map_2[symbol])[lowest_index]
                return answer


        """
        Get the best (i.e. lowest) price in the symbol's book that still has
        available volume.  If there is no quote on the symbol's book with
        available volume, return null.  Otherwise return a Quote object with
        all the fields set.  Don't return any quote which is past its
        expiration time, or has been removed.
        """

    def execute_trade(self, symbol, volume_requested) -> TradeResult:
        #edge case
        if self.map_2.get(symbol) is None:
            print("No SYMBOL exists, RETURNING NONE")
            return None
        trade_result_volume_executed=volume_requested
        #error nothing left

        #this is somewhat inefficient
        #a potential optimization would've used insertion sort for add_or_update_quote, based on the price

        currentDate = datetime.now().year  # edit would've used more specific dates and time if I had a clear idea of the data format, see email
        lowest_price_indexes=[]
        index=0
        totalvolume=0
        Total_price=0

        #filtering quote objects and then sorting based on price!
        for quoteObject in (self.map_2[symbol]):
            Total_price+=quoteObject.price
            #if( quoteObject.expiration_datetime> currentDate and quoteObject.available_volume>0):
            if( quoteObject.available_volume>0):
                totalvolume += quoteObject.available_volume
                lowest_price_indexes.append(tuple((index, quoteObject.price)))
            index=index+1

        if(volume_requested> totalvolume):
            trade_result_volume_executed=totalvolume

        lowest_price_indexes=Sort_Tuple(lowest_price_indexes) #sorting by best price, have index in front
        #print("printing")
       # print(lowest_price_indexes)
        volume_value=volume_requested
        trade_id=-1

        for price_index, price in lowest_price_indexes:
            sum=(self.map_2[symbol])[price_index].available_volume-volume_value
            if sum<0: #spill over, keep substracting
                (self.map_2[symbol])[price_index].available_volume=0
                volume_value=abs(sum)
            elif sum==0:
                trade_id=(self.map_2[symbol])[price_index].id
                break
            else: #sum>0
                print()
                (self.map_2[symbol])[price_index].AvailableVolume= sum
                trade_id=(self.map_2[symbol])[price_index].id
                break



        return TradeResult(trade_id,symbol, Total_price/len(self.map_2[symbol]),volume_requested,trade_result_volume_executed)
        #assuming ID, is the ID of the Quote Object last subtracted in the trade?
       #assuming symbol table value
       # assuming average price in symbol book
       # trade_result_volume_requested (assume that's given in the paramater)
       #  assuming total amount that's possible to excute


        #get avalible price,see if it's lower or higher
        #if still left, then move to the next index.. for loop


        """
        Request that a trade be executed. For the purposes of this interface,
        assume that the trade is a request to BUY, not sell. Do not trade an
        expired quotes.
        To Execute a trade:
        * Search available quotes of the specified symbol from best price to
        worst price.
        * Until the requested volume has been filled, use as much available
        volume as necessary (up to what is available) from each quote,
        subtracting the used amount from the available amount.

        For example, we have two quotes:
        #>>> {Price: 1.0, Volume: 1,000, AvailableVolume: 750}
        #>>> {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}

        After calling once for 500 volume, the quotes are:
       # >>> {Price: 1.0, Volume: 1,000, AvailableVolume: 250}
      3  >>> {Price: 2.0, Volume: 1,000, AvailableVolume: 1,000}

        And After calling this a second time for 500 volume, the quotes are:
       # >>> {Price: 1.0, Volume: 1,000, AvailableVolume: 0}
       # >>> {Price: 2.0, Volume: 1,000, AvailableVolume: 750}
        """

if __name__ == '__main__':
    print("hello")
    test= QuoteManager()
    quote1=Quote(1,10,200,1000,2020)
    quote2 = Quote(1, 100, 200, 1000, 2020)  #Intentionally Testing Error

    quote3 = Quote(2, 10, 2000, 100, 2021)
    test.add_or_update_quote(quote1)
    test.add_or_update_quote(quote2) #Intentionally Testing Error
    test.add_or_update_quote(quote3)
    test.printMaps()
    print()
    print("Testing Update")
    quote4 = Quote(2, 10, 4000, 100, 2021)
    test.add_or_update_quote(quote4)
    test.printMaps()

    print()
    print("Adding More Values")
    quote5 = Quote(3, 20, 3000, 2020, 2021)
    quote6= Quote(4, 20, 4000, 1500, 2021)
    quote7 = Quote(5, 20, 6000, 1200, 2021)
    test.add_or_update_quote(quote5)
    test.add_or_update_quote(quote6)
    test.add_or_update_quote(quote7)
    quote8 = Quote(6, 30, 3000, 3000, 2021)
    quote9 = Quote(7, 30, 100, 4000, 2021)
    quote10 = Quote(8, 30, 6000, 3000, 2021)
    test.add_or_update_quote(quote8)
    test.add_or_update_quote(quote9)
    test.add_or_update_quote(quote10)
    test.printMaps()

    print()
    print("REMOVING QUOTE 5 and 6 (they have ID's 3 and 4 respectively")
    test.remove_quote(quote5.id)
    test.remove_quote(quote6.id)
    test.printMaps()

    print()
    print("TESTING REMOVING NONEXISTENT QUOTE")
    test.remove_quote(1000)

    print()
    print("ADDING SOME NEW QUOTES NOW")
    quote11 = Quote(9, 20, 1, 150, 1999)
    quote12 = Quote(10, 20, 2, 120, 2040)
    quote13 = Quote(11, 20, 24, 140, 2040)

    quote14 = Quote(12, 40, 241, 1401, 2030)
    quote15 = Quote(13, 40, 2451, 14501, 2030)
    quote16 = Quote(14, 40, 2415, 1401, 2030)

    test.add_or_update_quote(quote11)
    test.add_or_update_quote(quote12)
    test.add_or_update_quote(quote13)
    test.add_or_update_quote(quote14)
    test.add_or_update_quote(quote15)
    test.add_or_update_quote(quote16)

    test.printMaps()

    print()
    print("TEST REMOVE ALL QUOTES: REMOVING EVERYTHING WITH SYMBOL 40")
    test.remove_all_quotes(40)
    test.printMaps()

    print()
    print("TESTING get_best_quote_with_available_volume ")
    test.printObject(test.get_best_quote_with_available_volume(20))
    print("TESTING get_best_quote_with_available_volume, with expected NULL value ")
    test.printObject(test.get_best_quote_with_available_volume(234234))

    print()
    print("TESTING execute_trade, (wiping out symbol table 10)")
    test.printTradeResult(test.execute_trade(10,4200) )
    test.printMaps()

    print()
    print("TESTING execute_trade, (price 6000 should only have values left)")
    test.printTradeResult(test.execute_trade(30, 9000))
    test.printMaps()









