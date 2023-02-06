#!/usr/bin/env python
# coding: utf-8

# author: Rahul Bhadani

class Ztransform:
    """
    Implementing a Ztransform using difference equation
    
    """
    def __init__(self, num, denom, P=3, **kwargs):
        """
        Parameters
        ----------

        num = a
        denom b

        n = size(a)
        m = size(b)

        y[k] = \sum_{i=0}^n (a_i x[k-i]) - \sum_{j=1}^m (b_j y[k-j]) )/b_0

        """
        

        self.a = num
        self.b = denom

        self.n = len(self.a)
        self.m = len(self.b)
        
        self.n_points_to_store = self.m

        if(self.n > self.m):
            n_points_to_store = self.n

        # always add one extra point to store than maximum number of coefficients
        self.n_points_to_store = self.n_points_to_store + 1

        if(P > self.n_points_to_store):
            self.n_points_to_store = P
        
        self.x = [0]*(self.n_points_to_store)
        self.y = [0]*(self.n_points_to_store)



    def control(self, x_new):

        """
        Produces the controller output 

        """

        # When a new x value comes in, insert at the right and pop one element from the left
        self.x.pop(0)
        self.x.append(x_new)
        
        AX_SUM = 0
        BX_SUM = 0


        for i in range(0, self.n):
        #   print(self.a[i], self.x[self.n_points_to_store - i-1])
            AX_SUM = AX_SUM + (self.a[i]*self.x[self.n_points_to_store - i - 1])


        for j in range(1, self.m):
        #   print(self.b[j], self.y[self.n_points_to_store - j-1])
            BX_SUM = BX_SUM + (self.b[j]*self.y[self.n_points_to_store - j - 1])
        
        y_new = (AX_SUM - BX_SUM)/self.b[0]

        self.y.pop(0)
        self.y.append(y_new)

        return y_new
