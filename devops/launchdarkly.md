
# launchdarkly

## Anonymous users

Anonymous users are identical to regular users, but they are not stored in your dashboard. You can designate any user as an anonymous user. One of the primary reasons that users are sent anonymously is to avoid unauthenticated users diluting the useful data in the user dashboard.

1. 本质上是  有游客的需求，无需登陆，验证的用户
    1. 在dashboard上面不可见。这样浏览用户的时候，看到的都是可用的用户
    2. 不会传递其他属性
3. 可以分配unique key做标识和区分

Must be a boolean. This attribute prevents the user's other attributes from being transmitted to the events endpoint, and prevents the user from showing up on the dashboard.
