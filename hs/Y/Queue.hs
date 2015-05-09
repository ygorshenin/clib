module Y.Queue (make, push, pop, null) where

import Control.Monad
import Control.Monad.ST
import Data.Array.ST
import Data.STRef
import Prelude hiding (null)

data Queue s a = Queue { values :: STArray s Int a
                       , front  :: STRef s Int
                       , rear   :: STRef s Int
                       }

make :: Int -> a -> ST s (Queue s a)
make size x = do v <- newArray (1, size) x
                 f <- newSTRef 1
                 r <- newSTRef 1
                 return $ Queue v f r

push :: Queue s a -> a -> ST s ()
push q x = do r <- readSTRef (rear q)
              writeArray (values q) r x
              writeSTRef (rear q) (r + 1)

pop :: Queue s a -> ST s a
pop q = do f <- readSTRef (front q)
           writeSTRef (front q) (f + 1)
           readArray (values q) f

null :: Queue s a -> ST s Bool
null q = liftM2 (==) (readSTRef (front q)) (readSTRef (rear q))
